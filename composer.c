#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include <xdo.h>

#include "composer.h"
#include "composer_gui.h"
#include "composer_event.h"

struct key_button_t KEYS[GROUP_SIZE] =
{
        {"", GDK_KEY_a},
        {"", GDK_KEY_s},
        {"", GDK_KEY_d},
        {"", GDK_KEY_f},
        {"", GDK_KEY_j},
        {"", GDK_KEY_k},
        {"", GDK_KEY_l},
        {"", GDK_KEY_y}
};

struct group_button_t GROUPS[GROUP_SIZE] =
{
        {       // brackets
                {"", GDK_KEY_q},
                {
                        GDK_KEY_bracketleft,
                        GDK_KEY_bracketright,
                        GDK_KEY_parenleft,
                        GDK_KEY_parenright,
                        GDK_KEY_less,
                        GDK_KEY_greater,
                        GDK_KEY_braceleft,
                        GDK_KEY_braceright
                }
        },
        {       // norwegian
                {"", GDK_KEY_w},
                {
                        GDK_KEY_bracketleft,
                        GDK_KEY_Oslash,
                        GDK_KEY_Aring,
                        GDK_KEY_AE,
                        GDK_KEY_less,
                        GDK_KEY_ae,
                        GDK_KEY_aring,
                        GDK_KEY_oslash
                }
        },
        {       // german
                {"", GDK_KEY_e},
                {
                        GDK_KEY_Adiaeresis,
                        GDK_KEY_adiaeresis,
                        GDK_KEY_parenleft,
                        GDK_KEY_Aring,
                        GDK_KEY_less,
                        GDK_KEY_greater,
                        GDK_KEY_braceleft,
                        GDK_KEY_braceright
                }
        },
        {       // french
                {"", GDK_KEY_r},
                {
                        GDK_KEY_eacute,
                        GDK_KEY_Eacute,
                        GDK_KEY_agrave,
                        GDK_KEY_Agrave,
                        GDK_KEY_less,
                        GDK_KEY_greater,
                        GDK_KEY_ccedilla,
                        GDK_KEY_egrave
                }
        },
        {       // brackets
                {"", GDK_KEY_u},
                {
                        GDK_KEY_bracketleft,
                        GDK_KEY_bracketright,
                        GDK_KEY_parenleft,
                        GDK_KEY_parenright,
                        GDK_KEY_less,
                        GDK_KEY_greater,
                        GDK_KEY_braceleft,
                        GDK_KEY_braceright
                }
        },
        {       // brackets
                {"", GDK_KEY_i},
                {
                        GDK_KEY_bracketleft,
                        GDK_KEY_bracketright,
                        GDK_KEY_parenleft,
                        GDK_KEY_parenright,
                        GDK_KEY_less,
                        GDK_KEY_greater,
                        GDK_KEY_braceleft,
                        GDK_KEY_braceright
                }
        },
        {       // brackets
                {"", GDK_KEY_o},
                {
                        GDK_KEY_bracketleft,
                        GDK_KEY_bracketright,
                        GDK_KEY_parenleft,
                        GDK_KEY_parenright,
                        GDK_KEY_less,
                        GDK_KEY_greater,
                        GDK_KEY_braceleft,
                        GDK_KEY_braceright
                }
        },
        {       // brackets
                {"", GDK_KEY_p},
                {
                        GDK_KEY_bracketleft,
                        GDK_KEY_bracketright,
                        GDK_KEY_parenleft,
                        GDK_KEY_parenright,
                        GDK_KEY_less,
                        GDK_KEY_greater,
                        GDK_KEY_braceleft,
                        GDK_KEY_braceright
                }
        },
};




//guint gl_key_to_send=0;


//struct group_button_t* gl_selected_group;
//int gl_selected_group_index=-1;

struct configuration_t configuration = {
        NULL,  // group
        NULL,  // key
        -1,    // index
        0,     // key to send
        input  // state
};


int get_group_index_from_key(guint keyval)
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (GROUPS[i].button.key_in == keyval) {
                        return i;
                }
        }
        return -1;
}


int get_key_index_from_key(guint keyval)
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (KEYS[i].key_in == keyval) {
                        return i;
                }
        }
        return -1;
}


// Sets global group variables. And activates group on GUI.
void set_active_group(int index) {
        configuration.selected_group = &GROUPS[index];
        configuration.selected_group_index = index;

        gui_select_group(index, KEYS, configuration.selected_group);
}



// Set key to send to global variable.
// This key will be sent right before this application terminates.
gboolean send_key_from_keyval(guint keyval) 
{

        int index = get_key_index_from_key(keyval);

        if (index>-1) {
                configuration.key_to_send = configuration.selected_group->keys_out[index];
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


// Load index from settings and set active group
void load_settings() 
{
        int index = 0;
        FILE *f = fopen("composer.rc", "r");
        if (f == NULL) {
                perror("fopen");
        } else {
                fscanf(f, "%d",&index);
                fclose(f);
        }
 
        if (index < GROUP_SIZE && index > -1) {
                set_active_group(index);
        } else {
                set_active_group(0);
        }
}


// Save index of selected group to file.
// TODO: replace this with gsettings
void save_settings()
{
        FILE *f = fopen("composer.rc", "w");
        if (f == NULL)	{
                perror("fopen");
        } else {
                fprintf(f, "%d\n", configuration.selected_group_index);
                fclose(f);
        }
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
        gtk_init(&argc, &argv);
        gui_create(GROUPS);
        load_settings();
        gtk_main();

        if (configuration.key_to_send != 0) {
                send_key(configuration.key_to_send);
        }

        save_settings();
        return 0;
}

