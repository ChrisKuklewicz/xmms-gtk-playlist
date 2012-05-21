// This is included in main.c and gtkplaylist.c

// Keep track of resources to free
static guint timerid;  // timer for watching xmms playlist changes
CListData *cld1=NULL,*cld2=NULL;

void init_widgets(int is_plugin)
{
    initialize_worker();
    if (NULL!=window1)
    {
        gtk_widget_show(window1);
    }
    else
    {
        window1 = create_window1 ();
        if (is_plugin)
        {
            // This seems to work, may leak memory
            // if you enable & disable the plugin over and over
            gtk_signal_connect (GTK_OBJECT (window1), "destroy",
                                GTK_SIGNAL_FUNC (gtk_widget_hide),
                                NULL);
            gtk_signal_connect (GTK_OBJECT (window1), "delete_event",
                                GTK_SIGNAL_FUNC (gtk_widget_hide),
                                NULL);
        }
        else
        {
            gtk_signal_connect (GTK_OBJECT (window1), "destroy",
                                GTK_SIGNAL_FUNC (gtk_main_quit),
                                NULL);
            gtk_signal_connect (GTK_OBJECT (window1), "delete_event",
                                GTK_SIGNAL_FUNC (gtk_false),
                                NULL);
        }
        myclist=GTK_CLIST(lookup_widget(window1,"clist1"));
        g_assert(NULL!=myclist);
        myprogress=GTK_PROGRESS(lookup_widget(window1,"progressbar1"));
        g_assert(NULL!=myprogress);
        mystatus=GTK_STATUSBAR(lookup_widget(window1,"statusbar1"));
        g_assert(NULL!=mystatus);
        togglewatch=GTK_TOGGLE_BUTTON(lookup_widget(window1,"tglWatch"));
        g_assert(NULL!=togglewatch);
        mycombosearch=GTK_COMBO(lookup_widget(window1,"combo_Search"));
        g_assert(NULL!=mycombosearch);
        mycomboentrysearch=GTK_ENTRY(lookup_widget(window1,"combo_entry_Search"));
        g_assert(NULL!=mycomboentrysearch);
        myclistsearch=GTK_CLIST(lookup_widget(window1,"clist2"));
        g_assert(NULL!=myclistsearch);

        gtk_combo_disable_activate(mycombosearch);

        statusid=gtk_statusbar_get_context_id(mystatus,"Main context");

        cld1=g_new0(CListData,1); //free
        gtk_object_set_user_data(GTK_OBJECT(myclist),(gpointer)cld1);
        clear_clist(myclist);

        cld2=g_new0(CListData,1); //free
        gtk_object_set_user_data(GTK_OBJECT(myclistsearch),(gpointer)cld2);
        clear_clist(myclistsearch);

        gtk_clist_set_column_auto_resize(myclist,COL_INDEX,TRUE);
        gtk_clist_set_column_auto_resize(myclistsearch,COL_INDEX,TRUE);
        gtk_clist_set_column_auto_resize(myclist,COL_TIME,TRUE);
        gtk_clist_set_column_auto_resize(myclistsearch,COL_TIME,TRUE);
        gtk_clist_set_column_auto_resize(myclist,COL_FILE,TRUE);
        gtk_clist_set_column_auto_resize(myclistsearch,COL_FILE,TRUE);
        gtk_clist_set_reorderable(myclist,TRUE);
        gtk_clist_set_reorderable(myclistsearch,TRUE);

        gtk_widget_show (window1);

        // Communicate with xmms
        // xmms_import_all();
        timerid=gtk_timeout_add(10*1000,&xmms_watcher,NULL); //remove later
    }
}

void destroy_widgets()
{
    gtk_timeout_remove(timerid);
    g_free(cld2);
    g_free(cld1);
    destroy_worker();
}









