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
extern GtkBuilder *builder;

extern "C"
{
	// buttons
	void onClick_PrevButton(GtkObject *Sender);
	void onClick_NextButton(GtkObject *Sender);
	void onClick_PlayButton(GtkObject *Sender);
	void onClick_StopButton(GtkObject *Sender);
	void onClick_VolUpButton(GtkObject *Sender);
	void onClick_VolDnButton(GtkObject *Sender);

	// status toggles
	void onToggle_Repeat(GtkObject *Sender);
	void onToggle_Random(GtkObject *Sender);
	void onToggle_Single(GtkObject *Sender);
	void onToggle_Consume(GtkObject *Sender);
	void onToggle_CrossFade(GtkObject *Sender);

	// list item
	void onClick_ListItem(GtkObject *Sender, gpointer ListIndex);
	
	// timer
	bool onTimer(void);
}

#endif /* __SIGNALS_H__ */
