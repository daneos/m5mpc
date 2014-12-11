/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__SIGNALS_H__)
#define __SIGNALS_H__

#include <gtk/gtk.h>
#include "MPDClient.h"

extern MPDClient *mpd;

extern "C"
{
	// buttons
	void onClick_PrevButton(void);
	void onClick_NextButton(void);
	void onClick_PlayButton(void);
	void onClick_StopButton(void);
	void onClick_VolUpButton(void);
	void onClick_VolDnButton(void);

	//timer
	void onTimer(GtkBuilder *builder);
}

#endif /* __SIGNALS_H__ */
