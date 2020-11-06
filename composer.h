#ifndef __COMPOSER_H__
#define __COMPOSER_H__


#include <gtk/gtk.h>


#define GROUP_SIZE 8


void set_to_group(int index);
gboolean send_key_from_keyval(guint keyval);
gboolean set_to_group_from_keyval(guint keyval);


/**
 *    Model for buttons
 *
 **/
struct button_t {
        // parent for all buttons
        gchar label[10];       // label that gets displayed to the user
        guint key_in;          // button to select key or key group
};


struct key_button_t {
        // send key button (second row)
        struct button_t button;
        guint key_out;        // key that gets sent
};


struct group_button_t {
      // group key button (first row)
      struct button_t     button;
      struct key_button_t group[GROUP_SIZE]; // key group thet gets selected
};




#endif
