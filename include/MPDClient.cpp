/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "MPDClient.h"

//-----------------------------------------------------------------------------
MPDClient::MPDClient(const char *address, const int port)
{
	this->c = mpd_connection_new(address, port, TIMEOUT);
	if(mpd_connection_get_error(this->c) != MPD_ERROR_SUCCESS)
		throw Exception(E_CONNECTION_FAILED);
	this->version = mpd_connection_get_server_version(this->c);
	mpd_command_list_begin(this->c, true);
	mpd_send_status(this->c);
	mpd_send_current_song(this->c);
	mpd_command_list_end(this->c);
}

//-----------------------------------------------------------------------------
MPDClient::~MPDClient()
{
	mpd_connection_free(this->c);
}

//-----------------------------------------------------------------------------
void MPDClient::Update(void)
{
	struct mpd_status *status;
	const struct mpd_audio_format *format;

	if((status = mpd_recv_status(this->c)) == NULL)
		throw Exception(E_STATUS_FAILED);
	this->Volume = mpd_status_get_volume(status);
	this->Repeat = mpd_status_get_repeat(status);
	this->Songs = mpd_status_get_queue_length(status);
	this->State = mpd_status_get_state(status);
	if(mpd_status_get_error(status) != NULL)
		throw Exception(E_STATUS_INCORRECT);
	if(this->State == MPD_STATE_PLAY || this->State == MPD_STATE_PAUSE)
	{
		this->SongNo = mpd_status_get_song_pos(status);
		this->Time = mpd_status_get_elapsed_time(status);
		this->TotalTime = mpd_status_get_total_time(status);
		this->Bitrate =  mpd_status_get_kbit_rate(status);
	}
	if((format = mpd_status_get_audio_format(status)) != NULL)
	{
		this->SampleRate = format->sample_rate;
		this->Bits = format->bits;
		this->Channels = format->channels;
	}

	mpd_status_free(status);
}

//-----------------------------------------------------------------------------
char *MPDClient::getVersion(void)
{
	static char vstr[10];
	snprintf(vstr, sizeof vstr, "%d.%d.%d", this->version[0], this->version[1], this->version[2]);
	return vstr;
}
