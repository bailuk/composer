#ifndef __COMPOSER_EVENT_H__
#define __COMPOSER_EVENT_H__

#include <glib/gi18n.h>
#include <gtk/gtk.h>

void on_group_button_toggled (GtkToggleButton *source, gpointer user_data);
void on_key_button_toggled (GtkToggleButton *source, gpointer user_data);
void on_change_key_in (GtkToggleButton *source, gpointer user_data);
void on_change_key_out (GtkToggleButton *source, gpointer user_data);

gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer data);
int on_focus_out(GtkWidget *w, void *p);

#endif

