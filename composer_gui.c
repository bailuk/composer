#include "composer.h"
#include "composer_gui.h"
#include "composer_event.h"

GtkWidget * gl_main_window;
GtkWidget* gl_group_buttons[GROUP_SIZE];
GtkWidget* gl_key_buttons[GROUP_SIZE];



GtkWidget* add_button(GtkWidget *bbox, int index);
void add_buttons(GtkWidget* bbox, GtkWidget* buttons[]);

GtkWidget* create_bbox (char *title, GtkWidget* buttons[]);
GtkWidget* create_vbox(GtkWidget *window, struct group_button_t groups[], GtkWidget* group_buttons[], GtkWidget* key_buttons[]);


void set_group_buttons(struct group_button_t groups[], GtkWidget* buttons[]);
void set_buttons(struct key_button_t keys[], GtkWidget* buttons[]);


void button_init_label(struct key_button_t *key);


GtkWidget*
add_button(GtkWidget *bbox, int index) 
{
        GtkWidget *result = gtk_toggle_button_new_with_label ("");
        gtk_container_add (GTK_CONTAINER (bbox), result);

        g_signal_connect (result, "clicked",
                    G_CALLBACK (on_button_toggled),
                    (void*)index);

        return result;
}


void add_buttons(GtkWidget* bbox, GtkWidget* buttons[]) 
{
        for (int i=0; i<GROUP_SIZE; i++) {
                buttons[i] = add_button(bbox, i);
        }
}


void button_init_group_label(struct group_button_t *group)
{
        guint key_in  = group->button.key_in;
        guint key_out = group->group[6].key_out;

        g_snprintf(group->button.label, sizeof(group->button.label), "%lc :%lc", key_in, key_out);
}


void button_init_label(struct key_button_t *key)
{
        guint key_in = key->button.key_in;
        guint key_out = key->key_out;

        g_snprintf(key->button.label, sizeof(key->button.label), "%lc :%lc", key_in, key_out);
}




void gui_select_group(int index, struct key_button_t keys[])
{
        gboolean state;

        for (int i=0; i<GROUP_SIZE; i++) {
                if (i == index) {
                        state = TRUE;
                } else {
                        state = FALSE;
                }

                if (gtk_toggle_button_get_active((GtkToggleButton*)gl_group_buttons[i]) != state) {
                        gtk_toggle_button_set_active((GtkToggleButton*)gl_group_buttons[i], state);
                }
        }
        set_buttons(keys, gl_key_buttons);
}


void set_buttons(struct key_button_t keys[], GtkWidget* buttons[]) {
        for (int i=0; i<GROUP_SIZE; i++) {
                button_init_label(&keys[i]);
                gtk_button_set_label(GTK_BUTTON(buttons[i]), keys[i].button.label);
        }
}

void set_group_buttons(struct group_button_t groups[], GtkWidget* buttons[])
{
        for (int i=0; i<GROUP_SIZE; i++) {
                button_init_group_label(&groups[i]);
                gtk_button_set_label(GTK_BUTTON(buttons[i]), groups[i].button.label);
        }
}




GtkWidget *
create_bbox (char *title, GtkWidget* buttons[])
{
        GtkWidget *result;
        GtkWidget *bbox;


        result = gtk_frame_new (title);
        bbox = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);

        gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
        gtk_container_add (GTK_CONTAINER (result), bbox);

        gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_EXPAND);
        gtk_box_set_spacing (GTK_BOX (bbox), 0);

        add_buttons(bbox, buttons);

        return result;
}


GtkWidget*
create_vbox(GtkWidget *window, struct group_button_t groups[], GtkWidget* group_buttons[], GtkWidget* key_buttons[])
{
        GtkWidget *result = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add (GTK_CONTAINER (window), result);
        gtk_container_set_border_width (GTK_CONTAINER (result), 10);


        gtk_box_pack_start (GTK_BOX (result),
                                create_bbox ("Group", group_buttons),
                                TRUE, TRUE, 5);

        gtk_box_pack_start (GTK_BOX (result),
                        create_bbox ("Key", key_buttons),
                        TRUE, TRUE, 5);

        set_group_buttons(groups, group_buttons);


        return result;
}




void gui_create(struct group_button_t groups[])
{
        gl_main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

        gtk_window_set_title (GTK_WINDOW (gl_main_window), "Composer");
        gtk_window_set_resizable (GTK_WINDOW (gl_main_window), FALSE);
        gtk_window_set_decorated (GTK_WINDOW (gl_main_window), FALSE);
        gtk_window_set_modal (GTK_WINDOW (gl_main_window), TRUE);
        gtk_window_set_skip_taskbar_hint (GTK_WINDOW (gl_main_window), TRUE);

        g_signal_connect (G_OBJECT (gl_main_window), "focus-out-event", G_CALLBACK (on_focus_out), NULL);
        g_signal_connect (G_OBJECT (gl_main_window), "key_press_event", G_CALLBACK (on_key_press), NULL);
        g_signal_connect(gl_main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        create_vbox(gl_main_window, groups, gl_group_buttons, gl_key_buttons);

        if (!gtk_widget_get_visible (gl_main_window)) {
                gtk_widget_show_all (gl_main_window);
        } else {
                gui_destroy();
        }
}




void gui_destroy() 
{
        gtk_widget_destroy(gl_main_window);
}


