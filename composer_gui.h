#ifndef __COMPOSER_GUI_H__
#define __COMPOSER_GUI_H__

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "composer.h"


void gui_init(int argc, char* argv[], struct context_t* context);

void gui_select_key(struct context_t* context);
void gui_select_group(struct context_t* context);

void gui_destroy();

void gui_set_config_label(struct context_t* config);
void gui_init_group_buttons(struct group_button_t groups[]);

const gchar* gui_entry_get_outkey();

#endif
