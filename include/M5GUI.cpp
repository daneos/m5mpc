/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#include "M5GUI.h"

//-----------------------------------------------------------------------------
M5GUI::M5GUI(const char *name, int *argc, char ***argv)
{
	this->WCount = -1;
	hildon_gtk_init(argc, argv);
	g_set_application_name(name);
	this->App = HILDON_PROGRAM(hildon_program_get_instance());
	this->MainWindow = GTK_WIDGET(hildon_window_new());
	gtk_window_set_title(GTK_WINDOW(this->MainWindow), name);
	hildon_program_add_window(this->App, HILDON_WINDOW(this->MainWindow));
	g_signal_connect(this->MainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	//hildon_window_set_app_menu(HILDON_WINDOW(this->win), this->menu);
	gtk_widget_show_all(GTK_WIDGET(this->MainWindow));
	hildon_program_set_can_hibernate(this->App, TRUE);
}

//-----------------------------------------------------------------------------
M5GUI::~M5GUI()
{

}

//-----------------------------------------------------------------------------
GtkWidget *M5GUI::getMainWindow(void)
{
	return this->MainWindow;
}

//-----------------------------------------------------------------------------
GtkWidget *M5GUI::getMainMenu(void)
{
	return this->MainMenu;
}

//-----------------------------------------------------------------------------
GtkWidget **M5GUI::getWidgetList(void)
{
	return this->WList;
}

//-----------------------------------------------------------------------------
int M5GUI::addWidget(GtkWidget *w)
{
	if(this->WCount < MAX_WIDGETS)
	{
		this->WList[++this->WCount] = w;
		return this->WCount;
	}
	return -1;
}

//-----------------------------------------------------------------------------
GtkWidget *M5GUI::getWidget(int index)
{
	return this->WList[index];
}
