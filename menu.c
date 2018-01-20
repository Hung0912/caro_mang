#include <gtk/gtk.h>

static void notice(char *text)
{
  	GtkWidget *window=gtk_window_new(GTK_WINDOW_POPUP);
  	GtkWidget *dialog;
  	dialog = gtk_message_dialog_new ((GtkWindow*)window, GTK_DIALOG_MODAL,
				   GTK_MESSAGE_INFO, GTK_BUTTONS_OK,text);
  	gtk_dialog_run(GTK_DIALOG(dialog));
  	gtk_widget_destroy(dialog);
}

static void button_connect_clicked(GtkWidget *widget,gpointer data)
{
	g_print("%s",gtk_entry_get_text(GTK_ENTRY(data)));	
	if (strcmp(gtk_button_get_label ((GtkButton *)widget), "Connect here")==0 )
	{
		notice("Connect success !");
		gtk_button_set_label ((GtkButton *)widget, "Disconnect");
	}else
	{
		notice("Ban da disconnect !");
		gtk_button_set_label ((GtkButton *)widget, "Connect here");
	}
	
}
static void button_invite_clicked(GtkWidget *widget,gpointer data)
{
		notice("Da gui loi moi thach dau!");
	
}

int main (int argc, char* argv[])
{
	gtk_init(&argc,&argv);
	GtkWidget *window;
	GtkWidget *button, *label,*image;
	GtkWidget *hbox;//horrizone box



	GtkWidget *vbox1,*hbox1,*hbox2,*entry,*button_connect;
	
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window,600,200);
	gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
//image
	image=gtk_image_new_from_file("icon/logo1.png");
	gtk_image_set_pixel_size (GTK_IMAGE(image),70);

//Nhap user va connect
//entry
	entry=gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY(entry),50);
	gtk_entry_set_placeholder_text (GTK_ENTRY(entry),"Nhap username vao day");
	button_connect=gtk_button_new_with_label("Connect here");
	g_signal_connect(button_connect,"clicked",G_CALLBACK(button_connect_clicked), entry);

	hbox2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start(GTK_BOX(hbox2),entry,1,1,10);	
	gtk_box_pack_start(GTK_BOX(hbox2),button_connect,0,0,10);


//Danh sach online
	GtkWidget *vbox2,*hbox3,*radio,*radio2,*label1,*button_invite;
	vbox2=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

	label = gtk_label_new("Nguoi choi dang online");
	gtk_box_pack_start(GTK_BOX(vbox2),label,0,0,0);
	radio=gtk_radio_button_new_with_label(NULL,"User 1");
	gtk_box_pack_start(GTK_BOX(vbox2),radio,0,0,0);


	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 2");
	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 3");
	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 4");
	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 5");
	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 6");
	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 7");
	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 8");
	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 9");
	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
//thach dau

	button_invite=gtk_button_new_with_label("Thach dau!");
	//gtk_button_set_alignment (GTK_BUTTON(button_invite),0.5,0.5);
	g_signal_connect(button_invite,"clicked",G_CALLBACK(button_invite_clicked), NULL);

	hbox3=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start(GTK_BOX(hbox3),vbox2,1,1,10);
	gtk_box_pack_start(GTK_BOX(hbox3),button_invite,1,1,10);
	
//producer
	GtkWidget *header;
	header = gtk_header_bar_new();
	gtk_header_bar_set_title (GTK_HEADER_BAR (header),"Nhom 9");
	gtk_header_bar_set_subtitle (GTK_HEADER_BAR (header),"Phan Bao Hung, Le Minh Duc, Nguyen Duc Toan");	
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header),
                                    	TRUE);

//box pack
	vbox1=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
	//gtk_box_pack_start(GTK_BOX(vbox1),header,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),image,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),hbox2,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),hbox3,0,0,10);
	gtk_container_add(GTK_CONTAINER(window),vbox1);
	
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}