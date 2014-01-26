#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

void cb_change_volume(GtkVScale * scale)
{
	gchar volume[14];
	gchar vol[14];  
	int cur_vol = gtk_range_get_value(GTK_RANGE(scale));
	sprintf(vol, "%d",cur_vol);
	snprintf(volume, sizeof volume, "%s%s", "mixer vol ", vol);
	system(volume);
}

void scale(GtkStatusIcon *status_icon)
{
	GtkWidget* box, *label;
	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 50, 150);
	gchar *endptr;
	gchar *cmd = "mixer -s | awk '$1~/^vol/{print $2}'";
	
	FILE* pipe = popen(cmd, "r");
	if(!pipe) printf("ERROR\n");
	char cur_vol[255];

	while(!feof(pipe))
	{
		if(fgets(cur_vol, 255, pipe) != NULL)
			printf("%s\n", cur_vol);	
	}
	printf("%s\n", cur_vol);
	pclose(pipe);

	long int vo = strtol(cur_vol, &endptr, 10);	
	GtkObject* adj = gtk_adjustment_new(vo, 0.0, 100.0, 1.0, 1.0, 0.0);
	GtkWidget *scale = gtk_vscale_new(GTK_ADJUSTMENT(adj));
	gtk_range_set_inverted(GTK_RANGE(scale), TRUE);
	box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);
	gtk_widget_show(box);
	label = gtk_label_new("Mixer");
	gtk_box_pack_start(GTK_BOX(box),scale, TRUE, TRUE, 0);
	gtk_signal_connect(GTK_OBJECT(scale), "value_changed",
			   G_CALLBACK(cb_change_volume),NULL);
	gtk_window_set_gravity(GTK_WINDOW(window), GDK_GRAVITY_SOUTH_EAST);
	GdkScreen *screen;
	GdkRectangle area;
	GtkOrientation orientation;

	if(gtk_status_icon_is_embedded(status_icon))
	{
		gtk_status_icon_get_geometry(status_icon,NULL,&area,NULL);
		gint icon_x = area.x;
		gint icon_y = area.y;
		gtk_window_move(GTK_WINDOW(window),icon_x,icon_y);
	} 

	gtk_widget_show_all(window);

}



void tray_icon_on_click(GtkStatusIcon *status_icon, 
                        gpointer user_data)
{
	scale(status_icon);
}

void tray_icon_on_menu(GtkStatusIcon *status_icon, guint button, 
                       guint activate_time, gpointer user_data)
{
	scale(status_icon);
}

static GtkStatusIcon *create_tray_icon() {
        GtkStatusIcon *tray_icon;

        tray_icon = gtk_status_icon_new();
        g_signal_connect(G_OBJECT(tray_icon), "activate", 
                         G_CALLBACK(tray_icon_on_click), NULL);
        g_signal_connect(G_OBJECT(tray_icon), 
                         "popup-menu",
                         G_CALLBACK(tray_icon_on_menu), NULL);
        gtk_status_icon_set_from_file(tray_icon,"/usr/local/share/freebsd_mix/sound.png");
        gtk_status_icon_set_tooltip(tray_icon, 
                                    "Mixer");
        gtk_status_icon_set_visible(tray_icon, TRUE);

        return tray_icon;
}

int main(int argc, char **argv) {
        GtkStatusIcon *tray_icon;

        gtk_init(&argc, &argv);
        tray_icon = create_tray_icon();
        gtk_main();

        return 0;
}
