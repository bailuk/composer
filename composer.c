#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include <xdo.h>

#include "composer.h"
#include "composer_gui.h"
#include "composer_event.h"
#include "composer_cfg.h"


struct context_t context;


gboolean change_key(guint keyval)
{
        if (context.change_key) {

                context.change_key = FALSE;

                if (keyval != GDK_KEY_Escape) {

                        if (context.selected_key > -1) {
                                context.keys[context.selected_key].key_in = keyval;
                        } else {

                                context.groups[context.selected_group].button.key_in = keyval;
                        }
                }

                context.selected_key = -1;

                gui_set_config_label (&context);
                gui_set_group_buttons(context.groups);
                gui_select_group (&context);
                return TRUE;
        }
        return FALSE;
}



gboolean change_key_out(const gchar* text)
{
        guchar uchar;
        struct group_button_t* group;

        gboolean result = FALSE;

        if (text != NULL &&
            context.selected_key > -1) {
                uchar = g_utf8_get_char(text);

                printf("string: %s\n", text);
                printf("strlen: %li\n",strlen(text));
                printf("uchar: %lc\n", uchar);
                printf("ucharv: %u\n", uchar);

                printf("selected group %i\n", context.selected_group);
                printf("selected key %i\n", context.selected_key);

                group = &context.groups[context.selected_group];
                group->keys_out[context.selected_key] = uchar;

                result = TRUE;
        }

        context.selected_key = -1;
        context.change_key = FALSE;

        return result;
}


int get_group_index_from_key(guint keyval)
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (context.groups[i].button.key_in == keyval) {
                        return i;
                }
        }
        return -1;
}




int get_key_index_from_key(guint keyval)
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (context.keys[i].key_in == keyval) {
                        return i;
                }
        }
        return -1;
}


void set_active_group(int index)
{
        context.change_key = FALSE;

        if (index < 0 || index >= GROUP_SIZE) {
                index = 0;
        }

        context.selected_group = index;
        gui_select_group(&context);
        gui_set_config_label (&context);
}

void set_active_key(int index)
{
        context.change_key = FALSE;

        if (index < 0 || index >= GROUP_SIZE) {
                index = -1;
        }

        //printf("key index: %i\n", index);
        context.selected_key = index;
        gui_select_key(&context);
        gui_set_config_label (&context);
}


// Set key to send to global variable.
// This key will be sent right before this application terminates.
gboolean send_key_from_keyval(guint keyval)
{

        int index = get_key_index_from_key(keyval);

        if (index>-1) {
                context.key_to_send = context.groups[context.selected_group].keys_out[index];
                return TRUE;
        }
        return FALSE;
}



gboolean set_active_group_from_keyval(guint keyval)
{
        int index = get_group_index_from_key(keyval);

        if (index > -1) {
                set_active_group(index);
                return TRUE;
        }
        return FALSE;
}


// Let xdo send key to window that has focus.
// This happend after gtk event loop has been terminated.
void send_key(guint key) 
{
        xdo_t* xdo = xdo_new(NULL);

        gchar* key_sym = gdk_keyval_name(key);
        gchar key_str[10];
        snprintf(key_str, sizeof(key_str), "%lc", key);


        printf("key_sym: %s\n",   key_sym);
        printf("key_str: %s\n",   key_str);
        printf("key_hex: 0x%X\n", key);
        printf("key_chr: %lc\n",  key);

        xdo_enter_text_window(xdo, CURRENTWINDOW, key_str, 0);
        xdo_free(xdo);
}


int main(int argc, char **argv)
{

        cfg_load(&context);
        gui_init(argc, argv, &context);

        if (context.key_to_send != 0) {
                send_key(context.key_to_send);
        }

        cfg_save(&context);
        return 0;
}


