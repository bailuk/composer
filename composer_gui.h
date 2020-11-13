#ifndef __COMPOSER_GUI_H__
#define __COMPOSER_GUI_H__

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "composer.h"


void gui_select_key(int index);
void gui_select_group(struct configuration_t* configuration);
void gui_create(struct configuration_t* configuration);
void gui_destroy();


#endif
