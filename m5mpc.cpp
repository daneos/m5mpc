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
void handle_input(MPDClient *mpd);
void print_playlist(MPDClient *mpd);

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
		return -2;
	}
}

//-----------------------------------------------------------------------------
int run(void)
{
	MPDClient *mpd = new MPDClient("10.96.0.1", 6600);
	welcome(mpd);
	bool quit = false;
	mpd->Update();
	print_playlist(mpd);
	while(!quit)
	{
		if(mpd->Update()) print_info(mpd);
		handle_input(mpd);
	}	
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
	printf("---------------------------------------------- %d\n", mpd->queue_version);
	printf("%s - %s (from %s)\n", mpd->Artist, mpd->SongTitle, mpd->Album);
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
	printf(" - %d/%d   Vol: %d%%\t\t", mpd->SongIndex+1, mpd->Songs, mpd->Volume);
	printf("%s | %s | %s | %s | %s \n",mpd->Repeat?"repeat":"",mpd->Random?"random":"",mpd->Single?"single":"",mpd->Consume?"consume":"",mpd->CrossFade?"crossfade":"");
	printf("%0.2d:%0.2d/%0.2d:%0.2d  @  %dkb/s\n", mpd->Time/60, mpd->Time%60, mpd->TotalTime/60, mpd->TotalTime%60, mpd->Bitrate);
	printf("%dHz   %dbit   %dch\n", mpd->SampleRate, mpd->Bits, mpd->Channels);
}

//-----------------------------------------------------------------------------
void handle_input(MPDClient *mpd)
{
	switch(getchar())
	{
		case 'p':
			mpd->TogglePlay();
			break;
		case 's':
			mpd->Stop();
			break;
		case 'u':
			mpd->UpdateDB(false, NULL);
			break;
		case 'e':
			mpd->Play();
			break;
		case 'w':
			mpd->Pause();
			break;
		case '+':
			mpd->setVolume(mpd->Volume + 1);
			break;
		case '-':
			mpd->setVolume(mpd->Volume - 1);
			break;
		case '>':
			mpd->Next();
			break;
		case '<':
			mpd->Previous();
			break;
		case '1':
			mpd->setRepeat(!mpd->Repeat);
			break;
		case '2':
			mpd->setRandom(!mpd->Random);
			break;
		case '3':
			mpd->setSingle(!mpd->Single);
			break;
		case '4':
			mpd->setConsume(!mpd->Consume);
			break;
		case '5':
			mpd->setCrossFade(!mpd->CrossFade);
			break;
		default:
			break;
	}
	getchar();
}

//-----------------------------------------------------------------------------
void print_playlist(MPDClient *mpd)
{
	printf("@\t%4.4s %40.40s %40.40s %40.40s\n", "ID", "Title", "Artist", "Album");
	for(int i = 0; i < mpd->Songs; i++)
	{
		printf("%s\t", i == mpd->SongIndex ? ">" : "");
		printf("%4.4d %-60.60s", i+1, mpd->CurrentPlaylist[i].Title);
		printf("%-60.60s %-60.60s\n", mpd->CurrentPlaylist[i].Artist, mpd->CurrentPlaylist[i].Album);
	}
}
