#include <glib/gi18n.h>
#include <gtk/gtk.h>


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>



#include "composer.h"
#include "composer_cfg.h"


int read_context(struct context_t*);
int write_context(struct context_t*);

char* get_home_dir();
char* get_conf_file();
char* _get_conf_file();

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


void cfg_load(struct context_t* context)
{
        int size = read_context(context);

        printf("size: %i - %i\n", size, (int)sizeof(struct context_t));

        if (size != sizeof (struct context_t)) {
                set_defaults(context);
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
}



int read_context(struct context_t* context) {
        int result = 0;
        FILE *f = fopen(get_conf_file(), "r");


        if (f == NULL) {
                perror("fopen");
        } else {
                result = fread(context, sizeof(struct context_t), 1, f);
                fclose(f);
        }
        return result * sizeof(struct context_t);
}


void cfg_save(struct context_t* context)
{
        write_context(context);
}



int write_context(struct context_t* context)
{
        int result = 0;

        FILE *f = fopen(get_conf_file(), "w");
        if (f == NULL)	{
                perror("fopen");
        } else {
                result = fwrite(context, sizeof(struct context_t), 1, f);
                fclose(f);
        }
        return result * sizeof(struct context_t);
}



char* gl_conf_file = NULL;


char* get_conf_file()
{
        if (gl_conf_file == NULL) {
                gl_conf_file = _get_conf_file();
        }
        return gl_conf_file;

}


char* _get_conf_file()
{
        const char* file = "/.composer.rc";

        char* home = get_home_dir();

        int size = strlen(file) + strlen(home) + 1;
        char* conf_file = malloc(size);

        strcpy(conf_file, home);
        strcat(conf_file, file);

        return conf_file;
}


char* get_home_dir()
{
        char *homedir;

        if ((homedir = getenv("HOME")) == NULL) {
                homedir = "/tmp";
        }

        return homedir;
}

