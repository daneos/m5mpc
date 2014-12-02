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
void welcome(MPDClient *mpd);
int run(void);

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
