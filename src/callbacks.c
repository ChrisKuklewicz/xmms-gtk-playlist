#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "xmms/xmmsctrl.h"
#include <stdio.h>

#include "worker.h"
#include "globals.h"

void
on_about_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    /* TODO */
}

void
on_clist1_click_column                 (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data)
{
    CListData *cld=(CListData*)gtk_object_get_user_data(GTK_OBJECT(clist));
    g_assert(NULL!=cld);
    if (column!=cld->sortcol)
    {
        cld->sortcol=column;
        gtk_clist_set_sort_column(clist,cld->sortcol);
    }
    else
    {
        if (GTK_SORT_ASCENDING==cld->sortdir)
        {
            cld->sortdir=GTK_SORT_DESCENDING;
        }
        else
        {
            cld->sortdir=GTK_SORT_ASCENDING;
        }
        gtk_clist_set_sort_type(clist,cld->sortdir);
    }
    gtk_clist_sort(clist);
}

void
on_clist1_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    gint index,temp;
    gchar *string;
    
    temp=gtk_clist_get_text(clist,row,0,&string);
    if (1==temp)
    {
        index=g_strtod(string,NULL)-1; // subtract 1
        lastindex=index;
        /* Note: event can be NULL */
        if (NULL!=event)
        {
            if (GDK_2BUTTON_PRESS==event->type)
            {
                if (xmms_remote_is_running(session))
                {
                    xmms_remote_stop(session);
                    xmms_remote_set_playlist_pos(session,index);
                    xmms_remote_play(session);
                }
            }
        }
    }
}

void
on_btnImport_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
    xmms_import_all();
}


void
on_btnPlay_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    if (xmms_remote_is_running(session))
    {
        xmms_remote_stop(session);
        xmms_remote_set_playlist_pos(session,lastindex);
        xmms_remote_play(session);
    }
}

void
on_btnStop_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
    if (xmms_remote_is_running(session))
    {
        xmms_remote_stop(session);
    }
}




gint  _compare_list_item_labels(gconstpointer a,gconstpointer b)
{
    gchar *sa,*sb;
    gtk_label_get (GTK_LABEL(GTK_BIN(a)->child),&sa);
    gtk_label_get (GTK_LABEL(GTK_BIN(b)->child),&sb);
    if ((NULL!=sa)&&(NULL!=sb))
    {
        return strcmp(sa,sb);
    }
    else
    {
        return 1;
    }
}

void
action_search()
{
    GtkListItem *item = NULL;
    GList *items = NULL;
    gchar *input=gtk_editable_get_chars(GTK_EDITABLE(mycomboentrysearch),0,-1); //free
    if ((NULL!=input) && (apply_search(input,myclist,myclistsearch)))
    {
        // See if already in list
        item=GTK_LIST_ITEM(gtk_list_item_new_with_label(input));
        items=gtk_container_children(GTK_CONTAINER (mycombosearch->list));
        if ((NULL==items) || 
            (NULL==g_list_find_custom(items,(gpointer)item,
                                      &_compare_list_item_labels)))
        {
            gtk_widget_show (GTK_WIDGET(item));
        //gtk_container_add (GTK_CONTAINER (mycombosearch->list), GTK_WIDGET(item));
            items=NULL;
            items=g_list_append (items,item); //free
            gtk_list_prepend_items(GTK_LIST(mycombosearch->list),items);
        }
    }
    gtk_entry_set_text(mycomboentrysearch,input);
    g_free(input);
}

void
on_combo_entry_Search_activate         (GtkEditable     *editable,
                                        gpointer         user_data)
{
    action_search();
}

void
on_btn_Search_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
    action_search();
}
