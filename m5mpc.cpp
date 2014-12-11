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
	GtkBuilder *builder = gtk_builder_new();
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

	// setup timer
	g_timeout_add(1000, (GSourceFunc)onTimer, builder);

	//mpd->Update();

	/*for(int i = 0; i < mpd->Songs; i++)
	{
		char *title0 = g_strdup_printf("%s", mpd->CurrentPlaylist[i].Title);
		char *subtitle0 = g_strdup_printf("%s - %s", mpd->CurrentPlaylist[i].Artist, mpd->CurrentPlaylist[i].Album);
		/* stop buttons being a mile wide */
		/*char *title = g_strndup(title0, 60);
		char *subtitle = g_strndup(subtitle0, 60);
		GtkWidget *button = hildon_button_new_with_text((HildonSizeType)(HILDON_SIZE_AUTO_WIDTH | HILDON_SIZE_FINGER_HEIGHT), HILDON_BUTTON_ARRANGEMENT_VERTICAL, title, subtitle);
		g_signal_connect(button, "clicked", G_CALLBACK(button_test), GINT_TO_POINTER(i));
		gtk_box_pack_end(GTK_BOX(gui->getWidget(vbox)), button, FALSE, FALSE, 0);
		g_free (title0);
		g_free (subtitle0);
		g_free (title);
		g_free (subtitle);
	}

	GtkWidget *PlayButton = hildon_button_new_with_text((HildonSizeType)(HILDON_SIZE_AUTO_WIDTH|HILDON_SIZE_FINGER_HEIGHT), HILDON_BUTTON_ARRANGEMENT_HORIZONTAL, "Play/Pause", NULL);
	g_signal_connect(PlayButton, "clicked", G_CALLBACK(toggle_play), NULL);
	gtk_box_pack_end(GTK_BOX(gui->getWidget(vbox2)), PlayButton, FALSE, FALSE, 0);

	hildon_pannable_area_add_with_viewport(HILDON_PANNABLE_AREA(gui->getWidget(pan)), gui->getWidget(vbox));
	gtk_container_add(GTK_CONTAINER(gui->getMainWindow()), gui->getWidget(pan));
	gtk_container_add(GTK_CONTAINER(gui->getMainWindow()), gui->getWidget(vbox2));*/
	//gtk_widget_show_all(GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow")));

	//g_object_unref(G_OBJECT(builder));
    gtk_widget_show(MainWindow);
    onTimer(builder);

	gtk_main();
	delete mpd;
	return 0;
}
