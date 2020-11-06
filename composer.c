#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include <xdo.h>

#include "composer.h"
#include "composer_gui.h"
#include "composer_event.h"


struct group_button_t GROUPS[] =
{
        {       // brackets
                {"", GDK_KEY_q},
                {
                        {{"", GDK_KEY_a}, GDK_KEY_bracketleft},
                        {{"", GDK_KEY_s}, GDK_KEY_bracketright},
                        {{"", GDK_KEY_d}, GDK_KEY_parenleft},
                        {{"", GDK_KEY_f}, GDK_KEY_parenright},
                        {{"", GDK_KEY_j}, GDK_KEY_less},
                        {{"", GDK_KEY_k}, GDK_KEY_greater},
                        {{"", GDK_KEY_l}, GDK_KEY_braceleft},
                        {{"", GDK_KEY_odiaeresis}, GDK_KEY_braceright }
                }
        },
        {       // norwegian
                {"", GDK_KEY_w},
                {
                        {{"", GDK_KEY_a}, GDK_KEY_bracketleft},
                        {{"", GDK_KEY_s}, GDK_KEY_Oslash},
                        {{"", GDK_KEY_d}, GDK_KEY_Aring},
                        {{"", GDK_KEY_f}, GDK_KEY_AE},
                        {{"", GDK_KEY_j}, GDK_KEY_less},
                        {{"", GDK_KEY_k}, GDK_KEY_ae},
                        {{"", GDK_KEY_l}, GDK_KEY_aring},
                        {{"", GDK_KEY_odiaeresis}, GDK_KEY_oslash }
                }
        },
        {       // german
                {"", GDK_KEY_e},
                {
                        {{"", GDK_KEY_a}, GDK_KEY_Adiaeresis},
                        {{"", GDK_KEY_s}, GDK_KEY_adiaeresis},
                        {{"", GDK_KEY_d}, GDK_KEY_parenleft},
                        {{"", GDK_KEY_f}, GDK_KEY_Aring},
                        {{"", GDK_KEY_j}, GDK_KEY_less},
                        {{"", GDK_KEY_k}, GDK_KEY_greater},
                        {{"", GDK_KEY_l}, GDK_KEY_braceleft},
                        {{"", GDK_KEY_odiaeresis}, GDK_KEY_braceright }
                }
        },
        {       // french
                {"", GDK_KEY_r},
                {
                        {{"", GDK_KEY_a}, GDK_KEY_eacute},
                        {{"", GDK_KEY_s}, GDK_KEY_Eacute},
                        {{"", GDK_KEY_d}, GDK_KEY_agrave},
                        {{"", GDK_KEY_f}, GDK_KEY_Agrave},
                        {{"", GDK_KEY_j}, GDK_KEY_less},
                        {{"", GDK_KEY_k}, GDK_KEY_greater},
                        {{"", GDK_KEY_l}, GDK_KEY_ccedilla},
                        {{"", GDK_KEY_odiaeresis}, GDK_KEY_egrave }
                }
        },
        {       // brackets
                {"", GDK_KEY_u},
                {
                        {{"", GDK_KEY_a}, GDK_KEY_bracketleft},
                        {{"", GDK_KEY_s}, GDK_KEY_bracketright},
                        {{"", GDK_KEY_d}, GDK_KEY_parenleft},
                        {{"", GDK_KEY_f}, GDK_KEY_parenright},
                        {{"", GDK_KEY_j}, GDK_KEY_less},
                        {{"", GDK_KEY_k}, GDK_KEY_greater},
                        {{"", GDK_KEY_l}, GDK_KEY_braceleft},
                        {{"", GDK_KEY_odiaeresis}, GDK_KEY_braceright }
                }
        },
        {       // brackets
                {"", GDK_KEY_i},
                {
                        {{"", GDK_KEY_a}, GDK_KEY_bracketleft},
                        {{"", GDK_KEY_s}, GDK_KEY_bracketright},
                        {{"", GDK_KEY_d}, GDK_KEY_parenleft},
                        {{"", GDK_KEY_f}, GDK_KEY_parenright},
                        {{"", GDK_KEY_j}, GDK_KEY_less},
                        {{"", GDK_KEY_k}, GDK_KEY_greater},
                        {{"", GDK_KEY_l}, GDK_KEY_braceleft},
                        {{"", GDK_KEY_odiaeresis}, GDK_KEY_braceright }
                }
        },
        {       // brackets
                {"", GDK_KEY_o},
                {
                        {{"", GDK_KEY_a}, GDK_KEY_bracketleft},
                        {{"", GDK_KEY_s}, GDK_KEY_bracketright},
                        {{"", GDK_KEY_d}, GDK_KEY_parenleft},
                        {{"", GDK_KEY_f}, GDK_KEY_parenright},
                        {{"", GDK_KEY_j}, GDK_KEY_less},
                        {{"", GDK_KEY_k}, GDK_KEY_greater},
                        {{"", GDK_KEY_l}, GDK_KEY_braceleft},
                        {{"", GDK_KEY_odiaeresis}, GDK_KEY_braceright }
                }
        },
        {       // brackets
                {"", GDK_KEY_p},
                {
                        {{"", GDK_KEY_a}, GDK_KEY_bracketleft},
                        {{"", GDK_KEY_s}, GDK_KEY_bracketright},
                        {{"", GDK_KEY_d}, GDK_KEY_parenleft},
                        {{"", GDK_KEY_f}, GDK_KEY_parenright},
                        {{"", GDK_KEY_j}, GDK_KEY_less},
                        {{"", GDK_KEY_k}, GDK_KEY_greater},
                        {{"", GDK_KEY_l}, GDK_KEY_braceleft},
                        {{"", GDK_KEY_odiaeresis}, GDK_KEY_braceright }
                }
        },

};




guint gl_key_out=0;


struct key_button_t *GL_current_keys;
int gl_current_index;


int key_button_index_from_key(guint keyval) 
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (GROUPS[i].button.key_in == keyval) {
                        return i;
                }
        }
        return -1;
}


struct key_button_t *
key_button_from_key(guint keyval) 
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (GL_current_keys[i].button.key_in == keyval) {
                        return &GL_current_keys[i];
                }
        }
        return NULL;
}


void set_to_group(int index) {
        struct key_button_t* keys = GROUPS[index].group;

        if (keys != NULL) {
                GL_current_keys = keys;
                gl_current_index = index;
                gui_select_group(index, GL_current_keys);
        }
}




gboolean send_key_from_keyval(guint keyval) 
{
        struct key_button_t *key = key_button_from_key(keyval);

        if (key) {
                gl_key_out = key->key_out;
                return TRUE;
        }
        return FALSE;
}



gboolean set_to_group_from_keyval(guint keyval) 
{
        int index = key_button_index_from_key(keyval);

        if (index > -1) {
                set_to_group(index);
                return TRUE;
        }
        return FALSE;
}


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
                set_to_group(index);
        } else {
                set_to_group(0);
        }
}



void save_settings()
{
        FILE *f = fopen("composer.rc", "w");
        if (f == NULL)	{
                perror("fopen");
        } else {
                fprintf(f, "%d\n", gl_current_index);
                fclose(f);
        }
}


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

        if (gl_key_out) {
                send_key(gl_key_out);
        }

        save_settings();
        return 0;
}

