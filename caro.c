#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Algo.h"
#define COLUMN 17
#define ROW 20

static int k=0;

static GtkWidget* wid[17][20], *window;
int m,n;
int komoku[COLUMN][ROW];


static void notice(char *text)
{
  	GtkWidget *window=gtk_window_new(GTK_WINDOW_POPUP);
  	GtkWidget *dialog;
  	dialog = gtk_message_dialog_new ((GtkWindow*)window, GTK_DIALOG_MODAL,
				   GTK_MESSAGE_INFO, GTK_BUTTONS_OK,text);
  	gtk_dialog_run(GTK_DIALOG(dialog));
  	gtk_widget_destroy(dialog);
}


static void nuoc_di(GtkWidget *widget,gpointer data)
{	
		GtkWidget *button;
	
		GtkWidget *button_image_x = gtk_image_new_from_file("icon/x2.png");
		GtkWidget *button_image_o = gtk_image_new_from_file("icon/o.png");
	//gtk_label_set_text(GTK_LABEL(data),"you clicked button")
		char pos[4];
		char* pos2;
		pos2=(char*)malloc(5*sizeof(char));
		int i = 0;
		int x,y,check;
		strcpy(pos2, gtk_widget_get_name(widget));
		//printf("%s\n", pos2);
		char *saveptr;
		char *foo, *bar;

		// checkWinning(pos);
		
		// char* token;
		// token = strtok(pos2," ");
		// while(token != NULL){
		// 	printf("%s\n", pos2);
		// 	request[i] = atoi(token);
		// 	//printf("%d\n",request[i]);
		// 	token = strtok(NULL, pos2);
		// 	i++;
		// }
		//printf("%d %d\n", request[0], request[1]);
		
		foo = strtok_r(pos2, " ", &saveptr);//printf("%d ",atoi(foo));
		bar = strtok_r(NULL, " ", &saveptr);//printf("%d\n",atoi(bar));
		x=atoi(foo);y=atoi(bar);printf("%d %d\n",x,y);
		free(pos2);
	if (k == 0) {
		gtk_button_set_image ((GtkButton *)widget, button_image_x);
		//g_print("Button is pressed\n");
		gtk_widget_set_sensitive((GtkWidget *)widget,FALSE);
		// if(checkWinning(k, request[0], request[1], komoku))
			// printf("player %i win\n", k);
		komoku[x][y] = 1;
		check = checkWinning(1, x, y, komoku);printf("%d\n",check);
		if(check == 1){
			notice("Player 1 win!");
		}
		k=1;

		//g_print("%d",k);	
	}
	else {
		gtk_button_set_image ((GtkButton *)widget, button_image_o);
		//g_print("Button is pressed\n");
		gtk_widget_set_sensitive((GtkWidget *)widget,FALSE);
		// if(checkWinning(k, request[0], request[1], komoku))
			// printf("player %i win\n", k);
		komoku[x][y] = 2;
		check = checkWinning(2, x, y, komoku);printf("%d\n",check);
		if(check == 1)
		{
			notice("Player 2 win!");
		}
		k=0;
		//g_print("%d",k);
	}
}


static void menu_event(GtkWidget* widget,gpointer data)
{

}

int main (int argc, char* argv[])
{	
	int i,j;

	  //Tao lap ban co trong
	for (m = 0;m < COLUMN;m++) {
    	for (n = 0;n < ROW;n++) 
    		komoku[m][n] = 0;
	}
	const char* file[] = {"New","Open","Exit"};
	/*for(i=0;i<9;i++)
		for (j=0;j<9;j++)
			a[i][j]=NULL;
	*/
	gtk_init(&argc,&argv);
	GtkWidget 	   *vbox,*hbox,*separator,*button,*file_menu,*menu_bar,*menu_item;
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

	
	
//Thanh menu
	file_menu=gtk_menu_new();
	menu_bar=gtk_menu_bar_new();
	menu_item=gtk_menu_item_new_with_label("File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item),file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar),menu_item);
	for(i=0;i<3;i++)
	{
		menu_item=gtk_menu_item_new_with_label(file[i]);
		gtk_menu_shell_append(GTK_MENU_SHELL(file_menu),menu_item);
		g_signal_connect(menu_item,"activate",G_CALLBACK(menu_event),NULL);
		

	}
	gtk_box_pack_start(GTK_BOX(vbox),menu_bar,0,0,0);
//Ban co
	for(i=0;i<17;i++)
	{
		hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);		
		for(j=0;j<20;j++)
		{
			wid[i][j]=gtk_button_new();
			gtk_widget_set_size_request(wid[i][j],40,40);
			gtk_box_pack_start(GTK_BOX(hbox),wid[i][j],0,1,0);
			g_signal_connect(wid[i][j],"clicked",G_CALLBACK(nuoc_di),NULL);
	
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
	}
//Menu
		
	


	g_signal_connect(window,"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
