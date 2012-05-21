#include "globals.h"

/* UGLY GLOBALS */
GtkWidget *window1=NULL;
GtkCList *myclist=NULL;
GtkProgress *myprogress=NULL;
GtkStatusbar *mystatus=NULL;
GtkToggleButton *togglewatch=NULL;
GtkCombo *mycombosearch=NULL;
GtkEntry *mycomboentrysearch=NULL;
GtkCList *myclistsearch=NULL;

guint statusid=0;  // for status bar
gint num=0;        // # of entries in main list
gint sortcol=0;    // column main list is sorted by
GtkSortType sortdir=GTK_SORT_ASCENDING;
/* GTK_SORT_ASCENDING or GTK_SORT_DESCENDING */

gint session = 0;  /* XMMS Session # */
gint lastindex=0;

gint my_pid=0;

const gint COL_INDEX=0;
const gint COL_TITLE=1;
const gint COL_TIME=2;
const gint COL_PATH=3;
const gint COL_FILE=4;
const gint COL_NUMBER=5;

