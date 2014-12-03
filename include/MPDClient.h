/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__MPDCLIENT_H__)
#define __MPDCLIENT_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <mpd/playlist.h>

#include "Exception.h"

#define TIMEOUT 30000

typedef struct m5mpc_song
{
	char *Title;
	char *Album;
	char *Artist;
	int Index;
} Song;

class MPDClient
{
protected:
	struct mpd_connection *c;
	const unsigned int *version;
	bool strings_alloc;
	bool queue_alloc;
	
public:
	int queue_version; // move this to protected! 
	int Volume;
	int Repeat;
	int Songs;
	int SongIndex;
	int Time;
	int TotalTime;
	int Bitrate;
	int SampleRate;
	int Bits;
	int Channels;
	int State;
	char *SongTitle;
	char *Album;
	char *Artist;
	Song *CurrentPlaylist;
	MPDClient(const char *address, const int port);
	~MPDClient();
	bool Update(void);
	char *getVersion(void);
	void ErrorRecover(void);
	bool Play(void);
	bool Pause(void);
	bool Stop(void);
	bool TogglePlay(void);
	int UpdateDB(bool rescan, const char *path);
	bool setVolume(int vol);
	bool Next(void);
	bool Previous(void);
	bool fetchPlaylist(const char* name);
};

#endif /* __MPDCLIENT_H__ */
