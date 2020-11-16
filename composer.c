#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include <xdo.h>

#include "composer.h"
#include "composer_gui.h"
#include "composer_event.h"
#include "composer_cfg.h"


struct context_t configuration;


gboolean change_key(guint keyval)
{
        if (configuration.change_key) {

                configuration.change_key = FALSE;

                if (keyval != GDK_KEY_Escape) {

                        if (configuration.selected_key > -1) {
                                configuration.keys[configuration.selected_key].key_in = keyval;
                        } else {

                                configuration.groups[configuration.selected_group].button.key_in = keyval;
                        }
                }

                configuration.selected_key = -1;

                gui_set_config_label (&configuration);
                gui_set_group_buttons(configuration.groups);
                gui_select_group (&configuration);
                return TRUE;
        }
        return FALSE;
}


int get_group_index_from_key(guint keyval)
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (configuration.groups[i].button.key_in == keyval) {
                        return i;
                }
        }
        return -1;
}




int get_key_index_from_key(guint keyval)
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (configuration.keys[i].key_in == keyval) {
                        return i;
                }
        }
        return -1;
}


void set_active_group(int index)
{
        configuration.change_key = FALSE;

        if (index < 0 || index >= GROUP_SIZE) {
                index = 0;
        }

        configuration.selected_group = index;
        gui_select_group(&configuration);
        gui_set_config_label (&configuration);
}

void set_active_key(int index)
{
        configuration.change_key = FALSE;

        if (index < 0 || index >= GROUP_SIZE) {
                index = -1;
        }

        printf("key index: %i\n", index);
        configuration.selected_key = index;
        gui_select_key(&configuration);
        gui_set_config_label (&configuration);
}


// Set key to send to global variable.
// This key will be sent right before this application terminates.
gboolean send_key_from_keyval(guint keyval)
{

        int index = get_key_index_from_key(keyval);

        if (index>-1) {
                configuration.key_to_send = configuration.groups[configuration.selected_group].keys_out[index];
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

        cfg_load(&configuration);
        gui_init(argc, argv, &configuration);

        if (configuration.key_to_send != 0) {
                send_key(configuration.key_to_send);
        }

        cfg_save(&configuration);
        return 0;
}

