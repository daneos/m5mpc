/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__MPDCLIENT_H__)
#define __MPDCLIENT_H__

#include <stdio.h>

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>

#include "Exception.h"

#define TIMEOUT 30000

class MPDClient
{
protected:
	struct mpd_connection *c;
	const unsigned int *version;

public:
	int Volume;
	int Repeat;
	int Songs;
	int SongNo;
	int Time;
	int TotalTime;
	int Bitrate;
	int SampleRate;
	int Bits;
	int Channels;
	int State;
	MPDClient(const char *address, const int port);
	~MPDClient();
	void Update(void);
	char *getVersion(void);
};

#endif /* __MPDCLIENT_H__ */
