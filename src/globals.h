#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include <pthread.h>
#include <gtk/gtk.h>
#include <error.h>
#include <errno.h>

extern GtkWidget *window1;
extern GtkCList *myclist;
extern GtkProgress *myprogress;
extern GtkStatusbar *mystatus;
extern GtkToggleButton *togglewatch;
extern GtkCombo *mycombosearch;
extern GtkEntry *mycomboentrysearch;
extern GtkCList *myclistsearch;

extern guint statusid;  /// for status bar context
extern gint num;  /// last known playlist length

extern gint session;  /// xmms session parameter
extern gint lastindex;  /// last xmms index selected

extern const gint COL_INDEX;
extern const gint COL_TITLE;
extern const gint COL_PATH;
extern const gint COL_FILE;
extern const gint COL_TIME;
extern const gint COL_NUMBER;

typedef struct  { 
    gint sortcol;
    GtkSortType sortdir;
    /* GTK_SORT_ASCENDING or GTK_SORT_DESCENDING */
} CListData;

extern int errno;
extern int my_pid;  /// used by plug-in

#endif
