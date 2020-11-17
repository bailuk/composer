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
        int index = GPOINTER_TO_INT(user_data);

        if (event_lock == FALSE) {
                event_lock = TRUE;

                if (gtk_toggle_button_get_active (source) == TRUE) {
                        set_active_key(index);
                } else {
                        set_active_key(-1);
                }

                event_lock = FALSE;
        }
}


gboolean on_key_press (GtkWidget* widget, GdkEventKey* event, gpointer data)
{

        if (event->keyval == GDK_KEY_Escape) {
                gui_destroy();
                return FALSE;
        }

        if (change_key(event->keyval)) {
                return FALSE;
        }

        if (send_key_from_keyval(event->keyval)) {
                gui_destroy();
                return FALSE;
        }

        if(set_active_group_from_keyval(event->keyval)) {
                return FALSE;
        }

        return FALSE;
}

void on_change_key_in (GtkToggleButton *source, gpointer user_data)
{
        struct context_t* context = (struct context_t*) user_data;

        if (context->change_key == TRUE) {
                context->change_key = FALSE;
        } else {
                context->change_key = TRUE;
        }
        gui_set_config_label(context);
}


void on_change_key_out (GtkToggleButton *source, gpointer user_data)
{

        struct context_t* context = (struct context_t*) user_data;

        const gchar* text = gui_entry_get_outkey();
        gunichar uchar;



        if (context->selected_key > -1) {

                if (text != NULL) {
                        uchar = g_utf8_get_char(text);

                        printf("string: %s\n", text);
                        printf("strlen: %li\n",strlen(text));
                        printf("uchar: %lc\n", uchar);
                        printf("ucharv: %u\n", uchar);

                        printf("selected group %i\n", context->selected_group);
                        printf("selected key %i\n", context->selected_key);

                        struct group_button_t* group = &context->groups[context->selected_group];
                        group->keys_out[context->selected_key] = uchar;


                }
                context->selected_key = -1;
                context->change_key = FALSE;


                gui_select_group(context);
                gui_set_config_label(context);
        }
}


int on_focus_out(GtkWidget* w, void* p)
{
        gui_destroy();
        return FALSE;
}

