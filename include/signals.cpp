/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "signals.h"

//-----------------------------------------------------------------------------
void onClick_PrevButton(void)
{
	mpd->Previous();
	mpd->Update();
}

//-----------------------------------------------------------------------------
void onClick_NextButton(void)
{
	mpd->Next();
	mpd->Update();
}

//-----------------------------------------------------------------------------
void onClick_PlayButton(void)
{
	mpd->togglePlay();
	mpd->Update();
}

//-----------------------------------------------------------------------------
void onClick_StopButton(void)
{
	mpd->Stop();
	mpd->Update();
}

//-----------------------------------------------------------------------------
void onClick_VolUpButton(void)
{
	mpd->setVolume(mpd->Volume+1);
	mpd->Update();
}

//-----------------------------------------------------------------------------
void onClick_VolDnButton(void)
{
	mpd->setVolume(mpd->Volume-1);
	mpd->Update();
}

//-----------------------------------------------------------------------------
void onTimer(GtkBuilder *builder)
{
	mpd->Update();
	GtkWidget *w;

	// labels
	w = GTK_WIDGET(gtk_builder_get_object(builder, "TitleLabel"));
	gtk_label_set_text(GTK_LABEL(w), mpd->SongTitle);
	w = GTK_WIDGET(gtk_builder_get_object(builder, "ArtistLabel"));
	gtk_label_set_text(GTK_LABEL(w), mpd->Artist);
	w = GTK_WIDGET(gtk_builder_get_object(builder, "AlbumLabel"));
	gtk_label_set_text(GTK_LABEL(w), mpd->Album);

	// progress bar
	w = GTK_WIDGET(gtk_builder_get_object(builder, "ProgressBar"));
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(w), (double)mpd->Time/(double)mpd->TotalTime);
	char *progress = g_strdup_printf("%02d:%02d / %02d:%02d", mpd->Time/60, mpd->Time%60, mpd->TotalTime/60, mpd->TotalTime%60);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(w), progress);
	g_free(progress);

	// volume bar
	w = GTK_WIDGET(gtk_builder_get_object(builder, "VolBar"));
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(w), (double)mpd->Volume/100.0);
}
