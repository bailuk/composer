#ifndef __COMPOSER_GUI_H__
#define __COMPOSER_GUI_H__

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "composer.h"


void gui_select_group(int index, struct KeyButton keys[]);
void gui_create(struct KeyButton groups[]);
void gui_destroy();


#endif
