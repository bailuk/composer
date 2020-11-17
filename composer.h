#ifndef __COMPOSER_H__
#define __COMPOSER_H__


#include <gtk/gtk.h>


#define GROUP_SIZE 8


void set_active_group(int index);
void set_active_key(int index);

gboolean set_active_group_from_keyval(guint keyval);
gboolean send_key_from_keyval(guint keyval);

gboolean change_key(guint keyval);

//
// Model for buttons
//


struct key_button_t {
        gchar label[10];       // label that gets displayed to the user
        guint key_in;          // button to select key or key group
};


struct group_button_t {
      // group key button (first row)
      struct key_button_t button;
      guint keys_out[GROUP_SIZE];
};



struct context_t {
        struct key_button_t     keys[GROUP_SIZE];
        struct group_button_t   groups[GROUP_SIZE];

        int selected_group;
        int selected_key;
        guint key_to_send;

        gboolean change_key;
};


#endif
