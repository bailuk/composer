#include "composer.h"
#include "composer_gui.h"
#include "composer_event.h"

GtkWidget * gl_main_window;
GtkWidget* gl_group_buttons[GROUP_SIZE];
GtkWidget* gl_key_buttons[GROUP_SIZE];



GtkWidget* add_key_button(GtkWidget *bbox, int index);
void add_key_buttons(GtkWidget* bbox);


GtkWidget* add_group_button(GtkWidget *bbox, int index);
void add_group_buttons(GtkWidget* bbox);

GtkWidget* create_key_frame();
GtkWidget* create_group_frame();
GtkWidget* create_bbox(GtkWidget* parent);
GtkWidget* create_vbox(GtkWidget* window, struct group_button_t groups[]);


void set_group_buttons(struct group_button_t groups[]);
void set_key_buttons(struct key_button_t keys[], struct group_button_t* group);


void button_init_label(struct key_button_t *key, guint key_out);


GtkWidget*
add_group_button(GtkWidget *bbox, int index)
{
        GtkWidget* result = gtk_toggle_button_new_with_label ("");
        gtk_container_add (GTK_CONTAINER (bbox), result);

        g_signal_connect (result, "clicked",
                    G_CALLBACK (on_group_button_toggled),
                    GINT_TO_POINTER(index));

        return result;
}


void add_group_buttons(GtkWidget* bbox)
{
        for (int i=0; i<GROUP_SIZE; i++) {
                gl_group_buttons[i] = add_group_button(bbox, i);
        }
}


void add_settings_buttons(GtkWidget* bbox)
{
        GtkWidget* result = gtk_toggle_button_new_with_label ("Change input key");
        gtk_container_add (GTK_CONTAINER (bbox), result);

        result = gtk_toggle_button_new_with_label ("Change output key");
        gtk_container_add (GTK_CONTAINER (bbox), result);

}


GtkWidget*
add_key_button(GtkWidget *bbox, int index)
{
        GtkWidget* result = gtk_toggle_button_new_with_label ("");
        gtk_container_add (GTK_CONTAINER (bbox), result);

        g_signal_connect (result, "clicked",
                    G_CALLBACK (on_key_button_toggled),
                    GINT_TO_POINTER(index));

        return result;
}




void add_key_buttons(GtkWidget* bbox)
{
        for (int i=0; i<GROUP_SIZE; i++) {
                gl_key_buttons[i] = add_key_button(bbox, i);
        }
}


void button_init_group_label(struct group_button_t *group)
{
        guint key_in  = group->button.key_in;
        guint key_out = group->keys_out[6];

        g_snprintf(group->button.label, sizeof(group->button.label), "%lc :%lc", key_in, key_out);
}


void button_init_label(struct key_button_t *key, guint key_out)
{
        guint key_in = key->key_in;

        g_snprintf(key->label, sizeof(key->label), "%lc :%lc", key_in, key_out);
}


void gui_select_button(int index, GtkWidget* buttons[]) {
        gboolean state;

        for (int i=0; i<GROUP_SIZE; i++) {

                if (i == index) {
                        state = TRUE;
                } else {
                        state = FALSE;
                }

                gtk_toggle_button_set_active((GtkToggleButton*)buttons[i], state);
        }

}


void gui_select_key(int index)
{
        gui_select_button(index, gl_key_buttons);
}


void gui_select_group(int index, struct key_button_t keys[], struct group_button_t* group)
{
        gui_select_button(index, gl_group_buttons);
        gui_select_key(-1);
        set_key_buttons(keys, group);
}


// set label of key buttons
// keys[] is model of key row,
// group contains keys_out for selected group.
void set_key_buttons(struct key_button_t keys[], struct group_button_t* group) {
        for (int i=0; i<GROUP_SIZE; i++) {
                button_init_label(&keys[i], group->keys_out[i]);
                gtk_button_set_label(GTK_BUTTON(gl_key_buttons[i]), keys[i].label);
        }
}

// set label of group buttons
void set_group_buttons(struct group_button_t groups[])
{
        for (int i=0; i<GROUP_SIZE; i++) {
                button_init_group_label(&groups[i]);
                gtk_button_set_label(GTK_BUTTON(gl_group_buttons[i]), groups[i].button.label);
        }
}



GtkWidget*
create_bbox(GtkWidget* parent) {
        GtkWidget* result = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);

        gtk_container_set_border_width (GTK_CONTAINER (result), 5);
        gtk_container_add (GTK_CONTAINER (parent), result);

        gtk_button_box_set_layout (GTK_BUTTON_BOX (result), GTK_BUTTONBOX_EXPAND);
        gtk_box_set_spacing (GTK_BOX (result), 0);
        return result;
}

GtkWidget*
create_group_frame()
{
        GtkWidget *result = gtk_frame_new ("Group");
        add_group_buttons(create_bbox(result));

        return result;
}


GtkWidget*
create_key_frame()
{
        GtkWidget *result = gtk_frame_new ("Key");
        add_key_buttons(create_bbox(result));

        return result;
}


GtkWidget*
create_settings_frame()
{
        GtkWidget *result = gtk_frame_new ("Settings");
        add_settings_buttons(create_bbox(result));

        return result;
}

GtkWidget*
create_vbox(GtkWidget *window, struct group_button_t groups[])
{
        GtkWidget *result = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
        gtk_container_add (GTK_CONTAINER (window), result);
        gtk_container_set_border_width (GTK_CONTAINER (result), 10);

        gtk_box_pack_start (GTK_BOX (result),
                                create_group_frame(),
                                TRUE, TRUE, 5);

        gtk_box_pack_start (GTK_BOX (result),
                        create_key_frame(),
                        TRUE, TRUE, 5);


        gtk_box_pack_start (GTK_BOX (result),
                        create_settings_frame(),
                        TRUE, TRUE, 5);

        set_group_buttons(groups);

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

        create_vbox(gl_main_window, groups);

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


