#ifndef __COMPOSER_H__
#define __COMPOSER_H__


#include <gtk/gtk.h>


#define GROUP_SIZE 8


void set_active_group(int index);
gboolean set_active_group_from_keyval(guint keyval);
gboolean send_key_from_keyval(guint keyval);


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



#endif
