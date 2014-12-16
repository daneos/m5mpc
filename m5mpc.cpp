/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <hildon/hildon.h>
#include <gtk/gtk.h>

#include "include/version.h"
#include "include/MPDClient.h"
#include "include/Exception.h"
#include "include/signals.h"

//-----------------------------------------------------------------------------
int run(int argc, char **argv);

//-----------------------------------------------------------------------------
MPDClient *mpd;
GtkBuilder *builder;

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	try
	{
		return run(argc, argv);
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
int run(int argc, char **argv)
{
	mpd = new MPDClient("10.96.0.1", 6600);
	hildon_gtk_init(&argc, &argv);
	GError *err = NULL;

	// load GUI and connect signals
	builder = gtk_builder_new();
	if(!gtk_builder_add_from_file(builder, "gui/main_window.ui", &err))
	{
		fprintf(stderr, "GtkBuilder Error: %s\n", err->message);
		return -1;
	}
	GtkWidget *MainWindow;
	if((MainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"))) == NULL)
	{
		fprintf(stderr, "Gtk Error: Couldn\'t find MainWindow.\n");
		return -1;
	}
	g_signal_connect(MainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_builder_connect_signals(builder, NULL);

	// setup portrait availablity (for non-forced portrait mode)
	hildon_gtk_window_set_portrait_flags(GTK_WINDOW(MainWindow), HILDON_PORTRAIT_MODE_SUPPORT);

	// setup timer
	g_timeout_add(1000, (GSourceFunc)onTimer, NULL);
	onTimer();

	// show window and start app main loop
	gtk_widget_show_all(MainWindow);
	gtk_main();

	// clear everything
	g_object_unref(G_OBJECT(builder));
	delete mpd;
	
	return 0;
}
