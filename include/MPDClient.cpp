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
	mpd_status_free(this->status);
	mpd_connection_free(this->c);
}

//-----------------------------------------------------------------------------
void MPDClient::Update(void)
{
	if((this->status = mpd_recv_status(this->c)) == NULL)
		throw Exception(E_STATUS_FAILED);
}

//-----------------------------------------------------------------------------
char *MPDClient::getVersion(void)
{
	static char vstr[10];
	snprintf(vstr, sizeof vstr, "%d.%d.%d", this->version[0], this->version[1], this->version[2]);
	return vstr;
}
