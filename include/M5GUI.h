/*
 * m5mpc -- MPD client for Maemo 5
 * Copyright (C) 2014 daneos.
 * Released under the GNU GPL v2 license
 */

#if !defined(__M5GUI_H__)
#define __M5GUI_H__

#include <hildon/hildon.h>
#include <gtk/gtk.h>

#define MAX_WIDGETS 4096

class M5GUI
{
protected:
	HildonProgram *App;
	GtkWidget *MainWindow;
	GtkWidget *MainMenu;
	GtkWidget *WList[MAX_WIDGETS];
	int WCount;

public:
	M5GUI(const char *name, int *argc, char ***argv);
	~M5GUI();
	GtkWidget *getMainWindow(void);
	GtkWidget *getMainMenu(void);
	GtkWidget **getWidgetList(void);
	int addWidget(GtkWidget *w);
	GtkWidget *getWidget(int index);
};

#endif /* __M5GUI_H__ */
