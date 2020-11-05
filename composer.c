#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include <xdo.h>

#include "composer.h"
#include "composer_gui.h"
#include "composer_event.h"


// key groups
struct KeyButton BRACKETS[GROUP_SIZE] = {
{GDK_KEY_a, GDK_KEY_bracketleft,    "", NULL}, 
{GDK_KEY_s, GDK_KEY_bracketright,   "", NULL},
{GDK_KEY_d, GDK_KEY_parenleft,      "", NULL},
{GDK_KEY_f, GDK_KEY_parenright,     "", NULL},
{GDK_KEY_j, GDK_KEY_less,           "", NULL},
{GDK_KEY_k, GDK_KEY_greater,        "", NULL},
{GDK_KEY_l, GDK_KEY_braceleft,      "", NULL},
{GDK_KEY_odiaeresis, GDK_KEY_braceright,"", NULL}
};


struct KeyButton NORWEGIAN[GROUP_SIZE] = {
{GDK_KEY_a, GDK_KEY_bracketleft,    "", NULL}, 
{GDK_KEY_s, GDK_KEY_Oslash,         "", NULL},
{GDK_KEY_d, GDK_KEY_Aring,          "", NULL},
{GDK_KEY_f, GDK_KEY_AE,             "", NULL},
{GDK_KEY_j, GDK_KEY_ae,             "", NULL},
{GDK_KEY_k, GDK_KEY_aring,          "", NULL},
{GDK_KEY_l, GDK_KEY_Ooblique,       "", NULL},
{GDK_KEY_odiaeresis, GDK_KEY_oslash,"", NULL}
};


struct KeyButton GERMAN[GROUP_SIZE] = {
{GDK_KEY_a, GDK_KEY_Adiaeresis,    "", NULL},
{GDK_KEY_s, GDK_KEY_adiaeresis,    "", NULL}, 
{GDK_KEY_d, GDK_KEY_Aring,         "", NULL},
{GDK_KEY_f, GDK_KEY_AE,            "", NULL},
{GDK_KEY_j, GDK_KEY_Udiaeresis,    "", NULL},
{GDK_KEY_k, GDK_KEY_udiaeresis,    "", NULL},
{GDK_KEY_l, GDK_KEY_Odiaeresis,    "", NULL},
{GDK_KEY_odiaeresis, GDK_KEY_odiaeresis,"", NULL}
};

struct KeyButton FRENCH[GROUP_SIZE] = {
{GDK_KEY_a, GDK_KEY_eacute,         "", NULL}, 
{GDK_KEY_s, GDK_KEY_Eacute,         "", NULL},
{GDK_KEY_d, GDK_KEY_agrave,         "", NULL},
{GDK_KEY_f, GDK_KEY_Agrave,         "", NULL},
{GDK_KEY_j, GDK_KEY_ae,             "", NULL},
{GDK_KEY_k, GDK_KEY_aring,          "", NULL},
{GDK_KEY_l, GDK_KEY_ccedilla,       "", NULL},
{GDK_KEY_odiaeresis, GDK_KEY_egrave,"", NULL}
};

struct KeyButton CODE[GROUP_SIZE] = {
{GDK_KEY_a, GDK_KEY_dollar,         "", NULL}, 
{GDK_KEY_s, GDK_KEY_numbersign,     "", NULL},
{GDK_KEY_d, GDK_KEY_at,             "", NULL},
{GDK_KEY_f, GDK_KEY_asterisk,       "", NULL},
{GDK_KEY_j, GDK_KEY_ae,             "", NULL},
{GDK_KEY_k, GDK_KEY_aring,          "", NULL},
{GDK_KEY_l, GDK_KEY_Ooblique,       "", NULL},
{GDK_KEY_odiaeresis, GDK_KEY_oslash,"", NULL}
};

// key group row (top row)
struct KeyButton GROUPS[GROUP_SIZE] = {
{GDK_KEY_q, 0, "", BRACKETS},
{GDK_KEY_w, 0, "", CODE},
{GDK_KEY_e, 0, "", NORWEGIAN},
{GDK_KEY_r, 0, "", FRENCH},
{GDK_KEY_t, 0, "", GERMAN},
{GDK_KEY_u, 0, "", BRACKETS},
{GDK_KEY_i, 0, "", BRACKETS},
{GDK_KEY_o, 0, "", BRACKETS},
};


guint gl_key_out=0;


struct KeyButton *GL_current_keys;
int gl_current_index;


int key_button_index_from_key(guint keyval) 
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (GROUPS[i].key_in == keyval) {
                        return i;
                }
        }
        return -1;
}


struct KeyButton *
key_button_from_key(guint keyval) 
{
        for (int i=0; i< GROUP_SIZE; i++) {
                if (GL_current_keys[i].key_in == keyval) {
                        return &GL_current_keys[i];
                }
        }
        return NULL;
}


void set_to_group(int index) {
        struct KeyButton* keys = GROUPS[index].keys;

        if (keys != NULL) {
                GL_current_keys = keys;
                gl_current_index = index;
                gui_select_group(index, GL_current_keys);
        }
}




gboolean send_key_from_keyval(guint keyval) 
{
        struct KeyButton *key = key_button_from_key(keyval);

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

