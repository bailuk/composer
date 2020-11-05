#ifndef __COMPOSER_H__
#define __COMPOSER_H__


#include <gtk/gtk.h>


#define GROUP_SIZE 8

struct KeyButton {
        guint key_in;      // button to select key or key group
        guint key_out;     // key that gets sent

        gchar label[10];   // label that gets displayed to the user
        void *keys;        // key group thet gets selected
};

void set_to_group(int index);
gboolean send_key_from_keyval(guint keyval);
gboolean set_to_group_from_keyval(guint keyval);
 

#endif
