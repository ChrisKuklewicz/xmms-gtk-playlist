#ifndef __WORKER_H__
#define __WORKER_H__
#include <pthread.h>
#include "glib.h"

void xmms_import_all();

gint xmms_watcher(gpointer data);

void clear_clist(GtkCList *a_list);

void destroy_worker();

void initialize_worker();

gboolean apply_search(char *input,GtkCList *in_list,GtkCList *out_list);

#endif
