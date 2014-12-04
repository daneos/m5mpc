/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "MPDClient.h"

//-----------------------------------------------------------------------------
MPDClient::MPDClient(const char *address, const int port)
{
	this->strings_alloc = false;
	this->queue_alloc = false;
	this->c = mpd_connection_new(address, port, TIMEOUT);
	if(mpd_connection_get_error(this->c) != MPD_ERROR_SUCCESS)
		this->ErrorRecover();
	this->version = mpd_connection_get_server_version(this->c);
	this->queue_version = -1;
}

//-----------------------------------------------------------------------------
MPDClient::~MPDClient()
{
	mpd_connection_free(this->c);
}

//-----------------------------------------------------------------------------
bool MPDClient::Update(void)
{
	struct mpd_status *status;
	const struct mpd_audio_format *format;
	struct mpd_song *song;
	const char *buffer;

	// cleanup
	if(this->strings_alloc)
	{
		free(this->SongTitle);
		free(this->Album);
		free(this->Artist);
	}

	// status
	if((status = mpd_run_status(this->c)) == NULL)
	{
		this->ErrorRecover();
		return false;
	}
	this->Volume = mpd_status_get_volume(status);
	this->Repeat = mpd_status_get_repeat(status);
	this->Songs = mpd_status_get_queue_length(status);
	this->State = mpd_status_get_state(status);
	this->Repeat = mpd_status_get_repeat(status);
	this->Random = mpd_status_get_random(status);
	this->Single = mpd_status_get_single(status);
	this->Consume = mpd_status_get_consume(status);
	this->CrossFade = mpd_status_get_crossfade(status);
	if(mpd_status_get_error(status) != NULL)
	{
		this->ErrorRecover();
		return false;
	}
	if(this->State == MPD_STATE_PLAY || this->State == MPD_STATE_PAUSE)
	{
		this->SongIndex = mpd_status_get_song_pos(status);
		this->Time = mpd_status_get_elapsed_time(status);
		this->TotalTime = mpd_status_get_total_time(status);
		this->Bitrate =  mpd_status_get_kbit_rate(status);
		if((format = mpd_status_get_audio_format(status)) != NULL)
		{
			this->SampleRate = format->sample_rate;
			this->Bits = format->bits;
			this->Channels = format->channels;
		}

		// current song
		if((song = mpd_run_current_song(this->c)) == NULL)
		{
			this->ErrorRecover();
			return false;
		}
		// title
		if((buffer = mpd_song_get_tag(song, MPD_TAG_TITLE, 0)) == NULL)
			buffer = mpd_song_get_uri(song);
		this->SongTitle = (char*)malloc((strlen(buffer)+1)*sizeof(char));
		snprintf(this->SongTitle, strlen(buffer)+1, "%s", buffer);
		// album
		if((buffer = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0)) != NULL)
		{
			this->Album = (char*)malloc((strlen(buffer)+1)*sizeof(char));
			snprintf(this->Album, strlen(buffer)+1, "%s", buffer);
		}
		else
		{
			this->Album = (char*)malloc(sizeof(char));
			this->Album = '\0';
		}
		// artist
		if((buffer = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)) != NULL)
		{
			this->Artist = (char*)malloc((strlen(buffer)+1)*sizeof(char));
			snprintf(this->Artist, strlen(buffer)+1, "%s", buffer);
		}
		else
		{
			this->Artist = (char*)malloc(sizeof(char));
			this->Artist = '\0';
		}
		this->strings_alloc = true;

		mpd_song_free(song);
	}
	else
	{
		this->SongIndex = 0;
		this->Time = 0;
		this->TotalTime = 0;
		this->Bitrate = 0;
		this->SampleRate = 0;
		this->Bits = 0;
		this->Channels = 0;
		this->SongTitle = (char*)malloc(sizeof(char));
		this->SongTitle = '\0';
		this->Album = (char*)malloc(sizeof(char));
		this->Album = '\0';
		this->Artist = (char*)malloc(sizeof(char));
		this->Artist = '\0';
	}

	int qv = mpd_status_get_queue_version(status);
	if(qv != this->queue_version)
		if(this->fetchPlaylist(NULL))
			this->queue_version = qv;

	mpd_status_free(status);

	return true;
}

//-----------------------------------------------------------------------------
char *MPDClient::getVersion(void)
{
	static char vstr[10];
	snprintf(vstr, sizeof vstr, "%d.%d.%d", this->version[0], this->version[1], this->version[2]);
	return vstr;
}

//-----------------------------------------------------------------------------
void MPDClient::ErrorRecover(void)
{
	if(!mpd_connection_clear_error(this->c)) throw Exception(E_UNRECOVERABLE);
}

//-----------------------------------------------------------------------------
bool MPDClient::Play(void)
{
	return mpd_run_play(this->c);
}

//-----------------------------------------------------------------------------
bool MPDClient::Pause(void)
{
	return mpd_run_pause(this->c, true);
}

//-----------------------------------------------------------------------------
bool MPDClient::Stop(void)
{
	return mpd_run_stop(this->c);
}

//-----------------------------------------------------------------------------
bool MPDClient::TogglePlay(void)
{
	mpd_command_list_begin(this->c, true);
	mpd_send_toggle_pause(this->c);
	mpd_command_list_end(this->c);
	return mpd_response_finish(this->c);
}

//-----------------------------------------------------------------------------
int MPDClient::UpdateDB(bool rescan, const char *path)
{
	return rescan ? mpd_run_rescan(this->c, path) : mpd_run_update(this->c, path);
}

//-----------------------------------------------------------------------------
bool MPDClient::setVolume(int vol)
{
	if(vol < 0 || vol > 100) return false;
	return mpd_run_set_volume(this->c, vol);
}

//-----------------------------------------------------------------------------
bool MPDClient::Next(void)
{
	return mpd_run_next(this->c);
}

//-----------------------------------------------------------------------------
bool MPDClient::Previous(void)
{
	return mpd_run_previous(this->c);
}

//-----------------------------------------------------------------------------
bool MPDClient::fetchPlaylist(const char *name)
{
	struct mpd_song *song;
	const char *buffer;

	// if no playlist specified, fetch current queue
	if(name == NULL)
	{
		if(this->queue_alloc) free(this->CurrentPlaylist); // TODO: this should also free all string allocated inside songs
		this->CurrentPlaylist = (Song*)malloc(this->Songs*sizeof(Song));
		this->queue_alloc = true;

		mpd_command_list_begin(this->c, true);
		mpd_send_list_queue_meta(this->c);
		mpd_command_list_end(this->c);

		while((song = mpd_recv_song(this->c)) != NULL)
		{
			int id = mpd_song_get_pos(song);
			this->CurrentPlaylist[id].Index = id;

			// title
			if((buffer = mpd_song_get_tag(song, MPD_TAG_TITLE, 0)) == NULL)
				buffer = mpd_song_get_uri(song);
			this->CurrentPlaylist[id].Title = (char*)malloc((strlen(buffer)+1)*sizeof(char));
			snprintf(this->CurrentPlaylist[id].Title, strlen(buffer)+1, "%s", buffer);

			// album
			if((buffer = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0)) != NULL)
			{
				this->CurrentPlaylist[id].Album = (char*)malloc((strlen(buffer)+1)*sizeof(char));
				snprintf(this->CurrentPlaylist[id].Album, strlen(buffer)+1, "%s", buffer);
			}
			else
			{
				this->CurrentPlaylist[id].Album = (char*)malloc(sizeof(char));
				this->CurrentPlaylist[id].Album = '\0';
			}

			// artist
			if((buffer = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0)) != NULL)
			{
				this->CurrentPlaylist[id].Artist = (char*)malloc((strlen(buffer)+1)*sizeof(char));
				snprintf(this->CurrentPlaylist[id].Artist, strlen(buffer)+1, "%s", buffer);
			}
			else
			{
				this->CurrentPlaylist[id].Artist = (char*)malloc(sizeof(char));
				this->CurrentPlaylist[id].Artist = '\0';
			}

			mpd_song_free(song);
		}
	}
	else
	{
		// fetch other playlist
	}

	return mpd_response_finish(this->c);
}

//-----------------------------------------------------------------------------
bool MPDClient::setRepeat(bool v)
{
	return mpd_run_repeat(this->c, v);
}

//-----------------------------------------------------------------------------
bool MPDClient::setRandom(bool v)
{
	return mpd_run_random(this->c, v);
}

//-----------------------------------------------------------------------------
bool MPDClient::setSingle(bool v)
{
	return mpd_run_single(this->c, v);
}

//-----------------------------------------------------------------------------
bool MPDClient::setConsume(bool v)
{
	return mpd_run_consume(this->c, v);
}

//-----------------------------------------------------------------------------
bool MPDClient::setCrossFade(bool v)
{
	return mpd_run_crossfade(this->c, v);
}
