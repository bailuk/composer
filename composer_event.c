#include <stdio.h>

#include "composer_event.h"
#include "composer.h"
#include "composer_gui.h"


gboolean event_lock = FALSE;

void on_group_button_toggled (GtkToggleButton* source, gpointer user_data)
{
        int index = GPOINTER_TO_INT(user_data);
        if (event_lock == FALSE) {
                event_lock = TRUE;
                
                printf("on_group_button_toggled: %d\n", index);
                set_active_group(index);

                event_lock = FALSE;
        }
}



void on_key_button_toggled (GtkToggleButton* source, gpointer user_data)
{
        if (event_lock == FALSE) {
                event_lock = TRUE;

                if (gtk_toggle_button_get_active (source) == TRUE) {
                        gui_select_key(GPOINTER_TO_INT(user_data));
                }

                event_lock = FALSE;
        }
}


gboolean on_key_press (GtkWidget* widget, GdkEventKey* event, gpointer data)
{
    if (send_key_from_keyval(event->keyval) || 
            !set_active_group_from_keyval(event->keyval)) {
                    gui_destroy();


    }
    return FALSE;
}

void on_change_key_in (GtkToggleButton *source, gpointer user_data) {
        struct configuration_t* config = (struct configuration_t*) user_data;

        if (config->state != input) {
                config->state = input;
        } else {
                config->state = change_key_in;
        }

}


void on_change_key_out (GtkToggleButton *source, gpointer user_data) {
        struct configuration_t* config = (struct configuration_t*) user_data;

        if (config->state != input) {
                config->state = input;
        } else {
                config->state = change_key_out;
        }

}



int on_focus_out(GtkWidget* w, void* p) {
        gui_destroy();
        return FALSE;
}

