#include <stdio.h>

#include "composer_event.h"
#include "composer.h"
#include "composer_gui.h"


gboolean event_lock = FALSE;

void on_button_toggled (GtkToggleButton *source, gpointer user_data) {
        if (event_lock == FALSE) {
                event_lock = TRUE;
                
                if (gtk_toggle_button_get_active (source) == TRUE) {
                        set_to_group((int)user_data);
                }

                event_lock = FALSE;
        }
}



gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer data) 
{
    if (send_key_from_keyval(event->keyval) || 
            !set_to_group_from_keyval(event->keyval)) {
                    gui_destroy();


    }
    return FALSE;
}


int on_focus_out(GtkWidget *w, void *p) {
        gui_destroy();
        return FALSE;
}

