#include "xmms/xmmsctrl.h"
#include <stdio.h>
#include <gtk/gtk.h>
#include <regex.h>   // This worker.c file is the only one with regex.h
#include <string.h>  // need strlen, no g_strlen ?

#include "worker.h"
#include "globals.h"

char trans_icase[256];                // go to lower case, etc.
static regex_t pattern;     // Needs a regfree()
static gchar *row_buffer[5]; // Never needs any "free"
static gint did_init=0;

/** Assume stuff defined in globals.h has NOT been setup yet.  This
 *  sets up the regex pattern buffer.
 */
void initialize_worker()
{
    if (0==did_init)
    {
        /* Build trans_icase */
        int i;
        for (i = 0; i < 256; i++)
            trans_icase[i] = i;
        for (i = 'a'; i <= 'z'; i++)
            trans_icase[i] = i - ('a' - 'A');
        /* Treat underscores as identical to spaces */
        i='_';
        trans_icase[i]=' ';
        /* Treat dashes as identical to spaces */
        i='-';
        trans_icase[i]=' ';

    /* Try "Posix Extended" with case ignored */
        re_syntax_options=RE_SYNTAX_POSIX_EXTENDED;
        pattern.translate=&trans_icase[0];
        pattern.fastmap=0;
        pattern.buffer=0;
        pattern.allocated=0;
        did_init=1;
    }
}

/** Assume stuff defined in globals.h has been destroyed.  This frees
 * the regex pattern buffer.
 */
void destroy_worker()
{
    //    regfree(&pattern); // segfaults!
}


/** This empties the GtkCList and resets the user data and resets the
 * sort parameters.
 */
void clear_clist(GtkCList *a_list)
{
    CListData *cld=(CListData*)gtk_object_get_user_data(GTK_OBJECT(a_list));
    cld->sortcol=COL_INDEX;
    cld->sortdir=GTK_SORT_ASCENDING;
    gtk_clist_set_sort_column(a_list,cld->sortcol);
    gtk_clist_set_sort_type(a_list,cld->sortdir);
    gtk_clist_clear(a_list);
}

/** This is the awfully slow function that reads in the playlist from
 * the running Xmms session.  The progress bar is updated during this
 * operation.
 */
void xmms_import_all()
{
    int i;
    gint time,sec,min;
    const gint strlen=255;
    gchar** newrow = g_new0(gchar*,COL_NUMBER);  //free
    gchar* newstring;
    gint temp;
    gint step=1;
    guint statusmsgid;

    if (!xmms_remote_is_running(session))
    {
        return;
    }

    g_assert(COL_NUMBER==myclist->columns);
    g_assert(NULL!=newrow);
    newrow[COL_INDEX]=g_new0(gchar,strlen+1); //free
    newrow[COL_TITLE]=NULL;
    newrow[COL_PATH]=NULL;
    newrow[COL_FILE]=NULL;
    newrow[COL_TIME]=g_new0(gchar,strlen+1);  //free
    g_assert(NULL!=newrow[COL_INDEX]);
    g_assert(NULL!=newrow[COL_TIME]);

    statusmsgid=gtk_statusbar_push(mystatus,statusid,
                                   "Importing playlist...");  //remove
    gtk_clist_freeze(myclist);
    /* Erase old data in the lists */
    gtk_clist_clear(myclist);
    clear_clist(myclistsearch);
    /* Remember num */
    num=xmms_remote_get_playlist_length(session);

    gtk_progress_configure(myprogress,0,0,num);
    step=num/20;
    if (step<=0)
    {
        step=1;
    }
    while (gtk_events_pending())
    {
            gtk_main_iteration();
    }
    for (i=0; i<num; i++)
    {
        if (step-1 == i % step)
        {
            gtk_progress_set_value(myprogress,i);
            while (gtk_events_pending())
            {
                gtk_main_iteration();
            }
        }
        temp=g_snprintf(newrow[COL_INDEX],strlen,"%4i ",i+1);  // add 1
        newrow[COL_TITLE]=xmms_remote_get_playlist_title(session,i); //free
        if (NULL==newrow[COL_TITLE])
        {
            /* Playlist was altered while downloading, abort */
            /* Remember num */
            num=i;
            break;
        }
        newstring=xmms_remote_get_playlist_file(session,i); //free
        if (NULL!=newstring)
        {
            newrow[COL_PATH]=g_dirname(newstring); //free
            newrow[COL_FILE]=g_basename(newstring);  //NO free
        }
        else
        {
            newrow[COL_PATH]="no path";
            newrow[COL_FILE]="no file";
        }
        time=xmms_remote_get_playlist_time(session,i);
        min=(time /1000) / 60;
        sec=(time /1000) % 60;
        temp=g_snprintf(newrow[COL_TIME],strlen," %2i:%02i ",min,sec);

        temp=gtk_clist_append(myclist,newrow);
        /* Was used to debug
        for (j=0; j<COL_NUMBER; j++)
        {
            printf("%s\n",newrow[j]);
        }
        */
        g_free(newrow[COL_TITLE]);
        if (NULL!=newstring)
        {
            g_free(newrow[COL_PATH]); 
            newrow[COL_PATH]=NULL;
            g_free(newstring);
            newstring=NULL;
            newrow[COL_FILE]=NULL;
        }
    }
    g_free(newrow[COL_INDEX]);
    g_free(newrow[COL_TIME]);
    g_free(newrow);
    gtk_clist_sort(myclist);
    gtk_clist_thaw(myclist);
    gtk_statusbar_remove(mystatus,statusid,statusmsgid);
    gtk_progress_set_value(myprogress,0);
}

gint xmms_watcher(gpointer data)
{
    int newnum=0;
    if (xmms_remote_is_running(session))
    {
        if (gtk_toggle_button_get_active(togglewatch))
        {
            newnum=xmms_remote_get_playlist_length(session);
            if (newnum!=num)
            {
                xmms_import_all();
            }
        }
    }
    return TRUE;
}


/** This is only called from search_clist */
gboolean _append_row(GtkCList *in_list, GtkCList *out_list, gint row)
{
    gint column;
    for (column=0; column<in_list->columns; column++)
    {
        if (!gtk_clist_get_text(in_list,row,column,&row_buffer[column]))
        {
            return FALSE;
        }
        // g_log(G_LOG_DOMAIN,G_LOG_LEVEL_INFO,G_GNUC_PRETTY_FUNCTION " (row_buffer[column %i] %s)\n",column,row_buffer[column]);
    }
    gtk_clist_append(out_list,row_buffer);

    return TRUE;
}

/** This is only called from apply_search */
gboolean _search_clist(GtkCList *in_list, GtkCList *out_list)
{
    gint row,column;
    gboolean result=FALSE;
    char *text;
    gint length;
    gint found;
    gint step;
    
    g_assert(NULL!=in_list);
    g_assert(NULL!=out_list);
    g_assert(in_list->columns==out_list->columns);
    g_assert(COL_NUMBER>=in_list->columns);

    gtk_clist_freeze(out_list);
    gtk_progress_configure(myprogress,0,0,in_list->rows);
    step=num/20;
    if (step<=0)
    {
        step=1;
    }
    while (gtk_events_pending())
    {
            gtk_main_iteration();
    }

    for (row=0; row<in_list->rows; row++)
    {
        if (step-1 == row % step)
        {
            gtk_progress_set_value(myprogress,row);
            while (gtk_events_pending())
            {
                gtk_main_iteration();
            }
        }
        for (column=0; column<in_list->columns; column++)
        {
            if (!gtk_clist_get_text(in_list,row,column,&text))
            {
                return FALSE;
            }
            length=strlen(text);   /* signed int vs size_t */
            if (0<length)
            {
                found=re_search(&pattern,text,length,0,length-1,0);
                if (0<=found)
                {
                    if (!result)
                    {
                        result=TRUE;
                        gtk_clist_clear(out_list);
                    }
                    if (!_append_row(in_list,out_list,row))
                    {
                        return FALSE;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (-1!=found)
                {
                    return FALSE; // error occured
                }
            }
            else
            {
                return FALSE; // overflow string length!
            }
        }
    }
    gtk_progress_set_value(myprogress,0);
    gtk_clist_thaw(out_list);
    return result;
}

/** 
 * This returns TRUE if at least one match was found,
 * and FALSE if there was an error or no match found
 */
gboolean apply_search(char *input,GtkCList *in_list,GtkCList *out_list)
{
    const char *error;
    gint length=strlen(input);   /* signed int vs size_t */
    guint statusmsgid;
    gboolean result=FALSE;

    statusmsgid=gtk_statusbar_push(mystatus,statusid,
                                   "Searching playlist...");  //remove

    if ((0<length) && (length<=1024))
    {
        error=re_compile_pattern(input,length,&pattern);
        if (NULL==error)
        {
            result=_search_clist(in_list,out_list);
        }
    }
    gtk_statusbar_remove(mystatus,statusid,statusmsgid);
    return result;
}
