#include "composer.h"
#include "composer_gui.h"
#include "composer_event.h"

GtkWidget* gl_main_window;
GtkWidget* gl_group_buttons[GROUP_SIZE];
GtkWidget* gl_key_buttons[GROUP_SIZE];

GtkWidget* gl_config_key_in;
GtkWidget* gl_config_key_out;
GtkWidget* gl_config_key_out_entry;


void gui_create(struct context_t* configuration);

GtkWidget* add_key_button(GtkWidget *bbox, int index);
void add_key_buttons(GtkWidget* bbox);



GtkWidget* add_group_button(GtkWidget *bbox, int index);
void add_group_buttons(GtkWidget* bbox);

GtkWidget* create_key_frame();
GtkWidget* create_group_frame();
GtkWidget* create_bbox(GtkWidget* parent);
GtkWidget* create_vbox(GtkWidget* window, struct context_t* configuration);


void init_key_buttons(struct key_button_t keys[], struct group_button_t* group);


void button_init_label(struct key_button_t *key, guint key_out);



void gui_init(int argc, char* argv[], struct context_t* context) {
        gtk_init(&argc, &argv);
        gui_create(context);
        gui_select_group(context);
        gtk_main();
}



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


void add_settings_buttons(GtkWidget* bbox, struct context_t* config)
{
        gl_config_key_in = gtk_button_new_with_label ("Change input key");
        gtk_container_add (GTK_CONTAINER (bbox), gl_config_key_in);
        g_signal_connect (gl_config_key_in, "clicked",
                    G_CALLBACK (on_change_key_in),
                    config);

        gl_config_key_out_entry = gtk_entry_new();
        gtk_container_add(GTK_CONTAINER(bbox), gl_config_key_out_entry);


        gl_config_key_out = gtk_button_new_with_label ("Change output key");
        gtk_container_add (GTK_CONTAINER (bbox), gl_config_key_out);
        g_signal_connect (gl_config_key_out, "clicked",
                    G_CALLBACK (on_change_key_out),
                    config);
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


void gui_select_key(struct context_t* context)
{
        gui_select_button(context->selected_key, gl_key_buttons);
}


void gui_select_group(struct context_t* context)
{
        gui_select_button(context->selected_group, gl_group_buttons);
        set_active_key(-1);

        init_key_buttons(context->keys, &context->groups[context->selected_group]);

}


void init_key_buttons(struct key_button_t keys[], struct group_button_t* group) {
        for (int i=0; i<GROUP_SIZE; i++) {
                init_key_label(&keys[i], group->keys_out[i]);
                gtk_button_set_label(GTK_BUTTON(gl_key_buttons[i]), keys[i].label);
        }
}

// set label of group buttons
void gui_init_group_buttons(struct group_button_t groups[])
{
        for (int i=0; i<GROUP_SIZE; i++) {
                init_group_label(&groups[i]);
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
create_settings_frame(struct context_t* config)
{
        GtkWidget *result = gtk_frame_new ("Settings");
        add_settings_buttons(create_bbox(result), config);

        return result;
}

GtkWidget*
create_vbox(GtkWidget* window, struct context_t* context)
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
                        create_settings_frame(context),
                        TRUE, TRUE, 5);

        gui_init_group_buttons(context->groups);

        return result;
}




void gui_create(struct context_t* config)
{
        // init global configuration object



        gl_main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

        gtk_window_set_title (GTK_WINDOW (gl_main_window), "Composer");
        gtk_window_set_resizable (GTK_WINDOW (gl_main_window), FALSE);
        gtk_window_set_decorated (GTK_WINDOW (gl_main_window), FALSE);
        gtk_window_set_modal (GTK_WINDOW (gl_main_window), TRUE);
        gtk_window_set_skip_taskbar_hint (GTK_WINDOW (gl_main_window), TRUE);

        g_signal_connect (G_OBJECT (gl_main_window), "focus-out-event", G_CALLBACK (on_focus_out), NULL);
        g_signal_connect (G_OBJECT (gl_main_window), "key_press_event", G_CALLBACK (on_key_press), NULL);
        g_signal_connect(gl_main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        create_vbox(gl_main_window, config);

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


char entry[5] = "";
void gui_set_config_label(struct context_t* context)
{
        char* label = "Change key";


        entry[0] = '\0';

        if (context->selected_key > -1) {
                g_snprintf(entry, sizeof(entry), "%lc", context->groups[context->selected_group].keys_out[context->selected_key]);
        }


        if (context->change_key) {
                if (context->selected_key > -1) {
                        label = "Press new key";

                } else {
                        label = "Press new group-key";
                }
        }

        //printf("key index %s, %i\n", label, context->selected_key);
        gtk_button_set_label(GTK_BUTTON(gl_config_key_in), label);


        gtk_entry_set_text(GTK_ENTRY(gl_config_key_out_entry), entry);
}


const gchar* gui_entry_get_outkey()
{
        return gtk_entry_get_text(GTK_ENTRY(gl_config_key_out_entry));
}
