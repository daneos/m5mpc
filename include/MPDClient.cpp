/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "MPDClient.h"

//-----------------------------------------------------------------------------
MPDClient::MPDClient(const char *address, const int port)
{
	this->allocated = false;
	this->c = mpd_connection_new(address, port, TIMEOUT);
	if(mpd_connection_get_error(this->c) != MPD_ERROR_SUCCESS)
		this->ErrorRecover();
	this->version = mpd_connection_get_server_version(this->c);
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
	if(this->allocated)
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
	if(mpd_status_get_error(status) != NULL)
	{
		this->ErrorRecover();
		return false;
	}
	if(this->State == MPD_STATE_PLAY || this->State == MPD_STATE_PAUSE)
	{
		this->SongNo = mpd_status_get_song_pos(status);
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
		buffer = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
		this->SongTitle = (char*)malloc((strlen(buffer)+1)*sizeof(char));
		snprintf(this->SongTitle, strlen(buffer)+1, "%s", buffer);
		//printf("%d %d %d %d\n", strlen(buffer), sizeof *buffer, sizeof *this->SongTitle, strlen(this->SongTitle));
		//strncpy(this->SongTitle, buffer, sizeof this->SongTitle);
		//this->SongTitle[strlen(buffer)] = '\0';
		// album
		buffer = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);
		this->Album = (char*)malloc((strlen(buffer)+1)*sizeof(char));
		snprintf(this->Album, strlen(buffer)+1, "%s", buffer);
		// artist
		buffer = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
		this->Artist = (char*)malloc((strlen(buffer)+1)*sizeof(char));
		snprintf(this->Artist, strlen(buffer)+1, "%s", buffer);
		this->allocated = true;
	}

	mpd_song_free(song);
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
