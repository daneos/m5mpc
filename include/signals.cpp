/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "signals.h"
int timer = 0;

//-----------------------------------------------------------------------------
void onClick_PrevButton(GtkObject *Sender)
{
	mpd->Previous();
	onTimer();
}

//-----------------------------------------------------------------------------
void onClick_NextButton(GtkObject *Sender)
{
	mpd->Next();
	onTimer();
}

//-----------------------------------------------------------------------------
void onClick_PlayButton(GtkObject *Sender)
{
	mpd->togglePlay();
	onTimer();
}

//-----------------------------------------------------------------------------
void onClick_StopButton(GtkObject *Sender)
{
	mpd->Stop();
	onTimer();
}

//-----------------------------------------------------------------------------
void onClick_VolUpButton(GtkObject *Sender)
{
	onTimer_VolUp();
}

//-----------------------------------------------------------------------------
void onClick_VolDnButton(GtkObject *Sender)
{
	onTimer_VolDn();
}

//-----------------------------------------------------------------------------
void onPressed_VolUpButton(GtkObject *Sender)
{
	timer = g_timeout_add(150, (GSourceFunc)onTimer_VolUp, NULL);
}

//-----------------------------------------------------------------------------
void onReleased_VolUpButton(GtkObject *Sender)
{
	g_source_remove(timer);
}

//-----------------------------------------------------------------------------
void onPressed_VolDnButton(GtkObject *Sender)
{
	timer = g_timeout_add(150, (GSourceFunc)onTimer_VolDn, NULL);
}

//-----------------------------------------------------------------------------
void onReleased_VolDnButton(GtkObject *Sender)
{
	g_source_remove(timer);
}

//-----------------------------------------------------------------------------
void onToggle_Repeat(GtkObject *Sender)
{
	mpd->setRepeat(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Sender)));
	onTimer();
}

//-----------------------------------------------------------------------------
void onToggle_Random(GtkObject *Sender)
{
	mpd->setRandom(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Sender)));
	onTimer();
}

//-----------------------------------------------------------------------------
void onToggle_Single(GtkObject *Sender)
{
	mpd->setSingle(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Sender)));
	onTimer();
}

//-----------------------------------------------------------------------------
void onToggle_Consume(GtkObject *Sender)
{
	mpd->setConsume(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Sender)));
	onTimer();
}

//-----------------------------------------------------------------------------
void onToggle_CrossFade(GtkObject *Sender)
{
	mpd->setCrossFade(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Sender)));
	onTimer();
}

//-----------------------------------------------------------------------------
void onClick_ListItem(GtkObject *Sender, gpointer ListIndex)
{
	mpd->playSongNo(GPOINTER_TO_INT(ListIndex));
	onTimer();
}

//-----------------------------------------------------------------------------
bool onTimer(void)
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
	char *progress = g_strdup_printf("%d:%02d / %d:%02d", mpd->Time/60, mpd->Time%60, mpd->TotalTime/60, mpd->TotalTime%60);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(w), progress);
	g_free(progress);

	// volume bar
	w = GTK_WIDGET(gtk_builder_get_object(builder, "VolBar"));
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(w), (double)mpd->Volume/100.0);

	// status labels
	w = GTK_WIDGET(gtk_builder_get_object(builder, "StateLabel"));
	gtk_label_set_text(GTK_LABEL(w), ((mpd->State==MPD_STATE_PLAY)?"Playing":((mpd->State==MPD_STATE_PAUSE)?"Paused":"Stopped")));
	
	w = GTK_WIDGET(gtk_builder_get_object(builder, "IndexLabel"));
	char *index = g_strdup_printf("%d / %d", mpd->SongIndex+1, mpd->Songs);
	gtk_label_set_text(GTK_LABEL(w), index);
	g_free(index);

	w = GTK_WIDGET(gtk_builder_get_object(builder, "BitrateLabel"));
	char *bitrate = g_strdup_printf("%dkb/s", mpd->Bitrate);
	gtk_label_set_text(GTK_LABEL(w), bitrate);
	g_free(bitrate);

	w = GTK_WIDGET(gtk_builder_get_object(builder, "SampleRateLabel"));
	char *samplerate = g_strdup_printf("%dHz", mpd->SampleRate);
	gtk_label_set_text(GTK_LABEL(w), samplerate);
	g_free(samplerate);

	w = GTK_WIDGET(gtk_builder_get_object(builder, "BitsLabel"));
	char *bits = g_strdup_printf("%dbit", mpd->Bits);
	gtk_label_set_text(GTK_LABEL(w), bits);
	g_free(bits);

	w = GTK_WIDGET(gtk_builder_get_object(builder, "ChannelsLabel"));
	char *channels = g_strdup_printf("%dch", mpd->Channels);
	gtk_label_set_text(GTK_LABEL(w), channels);
	g_free(channels);

	// status toggles
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "RepeatToggle")), mpd->Repeat);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "RandomToggle")), mpd->Random);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "SingleToggle")), mpd->Single);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ConsumeToggle")), mpd->Consume);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "CrossFadeToggle")), mpd->CrossFade);

	// continue timer
	return true;
}

//-----------------------------------------------------------------------------
bool onTimer_VolUp(void)
{
	mpd->setVolume(mpd->Volume+1);
	onTimer();
	return true;
}

//-----------------------------------------------------------------------------
bool onTimer_VolDn(void)
{
	mpd->setVolume(mpd->Volume-1);
	onTimer();
	return true;
}
