/*
 * This is the entry point for the xmms plugin version
 */
#include <pthread.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <xmms/plugin.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>      
#include <signal.h>
#include <string.h>

#include "interface.h"
#include "support.h"
#include "worker.h"
#include "globals.h"

// Share code with main.c via init.c.h
#include "init.c.h"

void init_plugin();
void cleanup_plugin();

GeneralPlugin gtkplaylist_gp =
{
    NULL,           /* handle */
    NULL,           /* filename */
    -1,         	/* xmms_session */
    "gtk-playlist 1.1",    /* Description */
    init_plugin,
    NULL,
    NULL,
    cleanup_plugin,
};

GeneralPlugin *get_gplugin_info(void)
{
    return &gtkplaylist_gp;
};

void init_plugin()
{
    session=gtkplaylist_gp.xmms_session;
    // add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
    // add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps");
    init_widgets(1);
    //    destroy_widgets();
};

void cleanup_plugin()
{
    gtk_widget_hide (window1);
    //    destroy_widgets();
    //gtk_widget_destroy (window1);
    //window1=NULL;
};


//typedef struct
//{
//	void *handle;		/* Filled in by xmms */
//	char *filename;		/* Filled in by xmms */
//	int xmms_session;	/* The session ID for attaching to the control socket */
//	char *description;	/* The description that is shown in the preferences box */
//	void (*init) (void);	/* Called when the plugin is enabled */
//	void (*about) (void);	/* Show the about box */
//	void (*configure) (void);
//	void (*cleanup) (void);	/* Called when the plugin is disabled or when xmms exits */
//}
//GeneralPlugin;




