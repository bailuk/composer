#ifndef __COMPOSER_GUI_H__
#define __COMPOSER_GUI_H__

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "composer.h"


void gui_select_group(int index, struct key_button_t keys[]);
void gui_create(struct group_button_t groups[]);
void gui_destroy();


#endif
