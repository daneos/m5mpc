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
#include "include/M5GUI.h"

//-----------------------------------------------------------------------------
int run(int argc, char **argv);
/*void welcome(MPDClient *mpd);
void print_info(MPDClient *mpd);
void handle_input(MPDClient *mpd);
void print_playlist(MPDClient *mpd);*/
MPDClient *global;
void button_test(GtkButton *button, gpointer i);

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
	MPDClient *mpd = new MPDClient("10.96.0.1", 6600);
	global = mpd;
	M5GUI *gui = new M5GUI("m5mpc", &argc, &argv);

	int vbox = gui->addWidget(GTK_WIDGET(gtk_vbox_new(FALSE, 0)));
	int pan = gui->addWidget(GTK_WIDGET(hildon_pannable_area_new()));

	mpd->Update();

	for(int i = 0; i < mpd->Songs; i++)
	{
		char *title0 = g_strdup_printf("%s", mpd->CurrentPlaylist[i].Title);
		char *subtitle0 = g_strdup_printf("%s - %s", mpd->CurrentPlaylist[i].Artist, mpd->CurrentPlaylist[i].Album);
		/* stop buttons being a mile wide */
		char *title = g_strndup(title0, 60);
		char *subtitle = g_strndup(subtitle0, 60);
		GtkWidget *button = hildon_button_new_with_text((HildonSizeType)(HILDON_SIZE_AUTO_WIDTH | HILDON_SIZE_FINGER_HEIGHT), HILDON_BUTTON_ARRANGEMENT_VERTICAL, title, subtitle);
		g_signal_connect(button, "clicked", G_CALLBACK(button_test), GINT_TO_POINTER(i));
		gtk_box_pack_end(GTK_BOX(gui->getWidget(vbox)), button, FALSE, FALSE, 0);
		g_free (title0);
		g_free (subtitle0);
		g_free (title);
		g_free (subtitle);
	}

	hildon_pannable_area_add_with_viewport(HILDON_PANNABLE_AREA(gui->getWidget(pan)), gui->getWidget(vbox));
	gtk_container_add(GTK_CONTAINER(gui->getMainWindow()), gui->getWidget(pan));
	gtk_widget_show_all(GTK_WIDGET(gui->getMainWindow()));

	gtk_main();
	delete gui;
	delete mpd;
	return 0;
}

/*
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
		case '~':
			mpd->playSongNo(100);
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
*/

void button_test(GtkButton *button, gpointer i)
{
	printf("callback: %d, %s\n", GPOINTER_TO_INT(i), global->playSongNo(GPOINTER_TO_INT(i))?"OK":"ERROR");
}
