#include <glib/gi18n.h>
#include <gtk/gtk.h>


#include <stdio.h>
#include <string.h>



#include "composer.h"
#include "composer_cfg.h"

int read_index();
void write_index(int index);

void set_defaults(struct context_t* context);

guint KEYS[GROUP_SIZE] ={
        GDK_KEY_a, GDK_KEY_s, GDK_KEY_d, GDK_KEY_f, GDK_KEY_j, GDK_KEY_k, GDK_KEY_l,GDK_KEY_y};

guint GKEYS[GROUP_SIZE] ={
        GDK_KEY_q, GDK_KEY_w, GDK_KEY_e, GDK_KEY_r, GDK_KEY_u, GDK_KEY_i, GDK_KEY_o,GDK_KEY_p};


guint BRACKETS[GROUP_SIZE] = {
        GDK_KEY_bracketleft,
        GDK_KEY_bracketright,
        GDK_KEY_parenleft,
        GDK_KEY_parenright,
        GDK_KEY_less,
        GDK_KEY_greater,
        GDK_KEY_braceleft,
        GDK_KEY_braceright
};

guint NORSK[GROUP_SIZE] = {
        GDK_KEY_bracketleft,
        GDK_KEY_Oslash,
        GDK_KEY_Aring,
        GDK_KEY_AE,
        GDK_KEY_less,
        GDK_KEY_ae,
        GDK_KEY_aring,
        GDK_KEY_oslash
};


// Load index from settings and set active group
void cfg_load(struct context_t* context)
{
        int index = read_index();



        set_defaults(context);

        if (index > -1 && index < GROUP_SIZE) {
                context->selected_group = index;
        }

}


void set_defaults(struct context_t* context)
{
        memset(context, '\0', sizeof(struct context_t));

        for (int i =0; i< GROUP_SIZE; i++) {
                context->groups[i].button.key_in = GKEYS[i];
                context->keys[i].key_in = KEYS[i];
        }
        memcpy(context->groups[0].keys_out, NORSK, sizeof(NORSK));
        memcpy(context->groups[1].keys_out, BRACKETS, sizeof(BRACKETS));

        context->state = input;
}


int read_index() {
        int result = 0;
        FILE *f = fopen("composer.rc", "r");


        if (f == NULL) {
                perror("fopen");
        } else {
                fclose(f);
        }
        return result;
}



void cfg_save(struct context_t* context)
{
        write_index(context->selected_group);
}



void write_index(int index)
{
        FILE *f = fopen("composer.rc", "w");
        if (f == NULL)	{
                perror("fopen");
        } else {
                fprintf(f, "%d\n", index);
                fclose(f);
        }
}



