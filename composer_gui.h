#ifndef __COMPOSER_GUI_H__
#define __COMPOSER_GUI_H__

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "composer.h"


void gui_init(int argc, char* argv[], struct context_t* context);

void gui_select_key(int index);
void gui_select_group(struct context_t* configuration);

void gui_destroy();

void gui_set_config_label(struct context_t* config);
void gui_set_group_buttons(struct group_button_t groups[]);


#endif
