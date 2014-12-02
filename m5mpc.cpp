/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "include/version.h"
#include "include/MPDClient.h"
#include "include/Exception.h"

//-----------------------------------------------------------------------------
int run(void);
void welcome(MPDClient *mpd);
void print_info(MPDClient *mpd);

//-----------------------------------------------------------------------------
int main(void)
{
	try
	{
		return run();
	}
	catch(Exception& e)
	{
		printf("%s\n", e.toString());
		return -1;
	}
	catch(...)
	{
		printf("Something weird happened, terminating...\n");
		return -1;
	}
}

//-----------------------------------------------------------------------------
int run(void)
{
	MPDClient *mpd = new MPDClient("10.96.0.1", 6600);
	welcome(mpd);
	mpd->Update();
	print_info(mpd);
	
	delete mpd;
	return 0;
}

//-----------------------------------------------------------------------------
void welcome(MPDClient *mpd)
{
	printf("m5mpc - MPD client for Maemo 5\nCopyright (C) 2014 daneos.\nReleased under GNU GPL v2 license.\n\n");
	printf("Client version   : %s\n", _VERSION);
	printf("Protocol version : %s\n", mpd->getVersion());
}

//-----------------------------------------------------------------------------
void print_info(MPDClient *mpd)
{
	printf("----------------------------------------------\n");
	switch(mpd->State)
	{
		case MPD_STATE_PLAY:
			printf("Playing");
			break;
		case MPD_STATE_PAUSE:
			printf("Paused");
			break;
		default:
			printf("Stopped");
	}
	printf(" - %d/%d   Vol: %d%%\n", mpd->SongNo, mpd->Songs, mpd->Volume);
	printf("%0.2d:%0.2d/%0.2d:%0.2d  @  %dkb/s\n", mpd->Time/60, mpd->Time%60, mpd->TotalTime/60, mpd->TotalTime%60, mpd->Bitrate);
	printf("%dHz   %dbit   %dch\n", mpd->SampleRate, mpd->Bits, mpd->Channels);
}