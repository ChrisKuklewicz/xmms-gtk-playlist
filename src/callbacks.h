//#include <pthread.h>
#include <gtk/gtk.h>


void
on_about_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_clist1_click_column                 (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data);

void
on_clist1_select_row                   (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btnImport_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnPlay_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnStop_clicked                     (GtkButton       *button,
                                        gpointer         user_data);


void
on_combo_entry_Search_activate         (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_btn_Search_clicked                  (GtkButton       *button,
                                        gpointer         user_data);
