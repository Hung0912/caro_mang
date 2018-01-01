#include <gtk/gtk.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <gdk/gdk.h>

#include "Algo.h"
#define COLUMN 17
#define ROW 20
#define MSG_SIZE 80
#define MAX_CLIENTS 95
#define MYPORT 7400


static int k=0;

static GtkWidget* wid[17][20], *window;
int m,n;
int komoku[COLUMN][ROW];
int sockfd;
int result;
char hostname[MSG_SIZE];
struct hostent *hostinfo;
struct sockaddr_in address;
char alias[MSG_SIZE];
int clientid;

int port;
int num_clients = 0;
int server_sockfd, client_sockfd;
struct sockaddr_in server_address;
int addresslen = sizeof(struct sockaddr_in);
int fd;
char fd_array[MAX_CLIENTS];
char* buffer1;
fd_set readfds, testfds, clientfds;
char msg[MSG_SIZE + 1];     
char kb_msg[MSG_SIZE + 10];

static void notice(char *text)
{
	GtkWidget *window=gtk_window_new(GTK_WINDOW_POPUP);
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new ((GtkWindow*)window, GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO, GTK_BUTTONS_OK,text);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void exitClient(int fd, fd_set *readfds, char fd_array[], int *num_clients){
	int i;

	close(fd);
    FD_CLR(fd, readfds); //clear the leaving client from the set
    for (i = 0; i < (*num_clients) - 1; i++)
    	if (fd_array[i] == fd)
    		break;          
    	for (; i < (*num_clients) - 1; i++)
    		(fd_array[i]) = (fd_array[i + 1]);
    	(*num_clients)--;
    }

    static void button_connect_clicked(GtkWidget *widget,gpointer data)
    {
    	g_print("%s",gtk_entry_get_text(GTK_ENTRY(data)));	
    	if (strcmp(gtk_button_get_label ((GtkButton *)widget), "Connect")==0 )
    	{
    		notice("Connect success !");
    		gtk_button_set_label ((GtkButton *)widget, "Disconnect");
    	}else
    	{
    		notice("Ban da disconnect !");
    		gtk_button_set_label ((GtkButton *)widget, "Connect");
    	}

    }
    static void button_invite_clicked(GtkWidget *widget,gpointer data)
    {

    	notice("Da gui loi moi thach dau!");

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
		check = checkWinning(1, x, y, komoku);//printf("%d\n",check);
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
		check = checkWinning(2, x, y, komoku);//printf("%d\n",check);
		if(check == 1)
		{
			notice("Player 2 win!");
		}
		k=0;
		//g_print("%d",k);
	}
	/*for (m = 0;m < COLUMN;m++) {
    	for (n = 0;n < ROW;n++) {
    		printf("%d",komoku[m][n]);
    	}
    	printf("\n");
	}*/

}



static void menu_event(GtkWidget* widget,gpointer data)
{

}

void initBoard(int argc, char*argv[]){
		int i,j;

	

	gtk_init(&argc,&argv);
	GtkWidget *hbox_lon,*vbox,*hbox,*separator,*button,*file_menu,*menu_bar,*menu_item;
	GtkWidget *label,*vbox1,*hbox1,*hbox2,*entry,*button_connect,*image;
	
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window,1000,1000);
	gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);

	
	
//Thanh menu
/*	file_menu=gtk_menu_new();
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
*/
//Ban co

	label=gtk_label_new("Fight!");
	gtk_box_pack_start(GTK_BOX(vbox),label,0,0,0);
	for(i=0;i<17;i++)
	{
		hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);		
		for(j=0;j<20;j++)
		{
			gchar pos[4];
			sprintf(pos, "%d %d", i, j);
			wid[i][j]=gtk_button_new();
			gtk_widget_set_name(wid[i][j],pos);

			gtk_widget_set_size_request(wid[i][j],40,40);
			gtk_box_pack_start(GTK_BOX(hbox),wid[i][j],0,1,0);
			
			g_signal_connect(wid[i][j],"clicked",G_CALLBACK(nuoc_di),NULL);

		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
	}

//Menu//image
	image=gtk_image_new_from_file("icon/logo1.png");
	gtk_image_set_pixel_size (GTK_IMAGE(image),70);

//Nhap user va connect
//entry
	entry=gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY(entry),50);
	gtk_entry_set_placeholder_text (GTK_ENTRY(entry),"Nhap username vao day");
	button_connect=gtk_button_new_with_label("Connect");
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



	label=gtk_label_new("____Nhom thuc hien____\n          Phan Bao Hung\n        Nguyen Duc Toan    \n            Le Minh Duc     ");
	
//box pack
	vbox1=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
	//gtk_box_pack_start(GTK_BOX(vbox1),header,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),image,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),hbox2,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),hbox3,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),label,0,0,0);
	//gtk_widget_set_size_request(vbox,600,400);
	//gtk_widget_set_size_request(vbox,600,400);
	

	hbox_lon=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	
	gtk_box_pack_start(GTK_BOX(hbox_lon),vbox1,0,0,0);
	gtk_box_pack_start(GTK_BOX(hbox_lon),vbox,0,0,0);
	
	g_signal_connect(window,"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	gtk_container_add(GTK_CONTAINER(window),hbox_lon);
	gtk_widget_show_all(window);

}

int main (int argc, char* argv[])
{	

	if( ! g_thread_supported() )
        g_thread_init( NULL );

    gdk_threads_init();

    gdk_threads_enter();
	

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

	initBoard(argc,argv);
	
	while(1){
		printf("1");
	}
	gtk_main();

	gdk_threads_leave();

//Client code


   return 0;

}
