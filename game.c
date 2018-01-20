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


static int k=1;
static int t=0;
	

static GtkWidget* wid[17][20], *window, *window_board;
GtkWidget *hbox_lon,*vbox,*hbox,*separator,*button,*button_quit;
GtkWidget *entry_moi,*hbox_list,*button_moi;
GtkWidget *label,*vbox1,*hbox1,*hbox2,*entry,*button_connect,*image;



GThread *thread;
GError *error = 0;
int m,n;
int komoku[COLUMN][ROW];
int sockfd;
int result;
char hostname[MSG_SIZE];
struct hostent *hostinfo;
struct sockaddr_in address;
char alias[MSG_SIZE];
int clientid;
int flag = 0;
char* name,move;

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

static void notice_end(char *text);

static void notice(char *text)
{
	GtkWidget *window=gtk_window_new(GTK_WINDOW_POPUP);
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new ((GtkWindow*)window, GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO, GTK_BUTTONS_OK,text);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}




void exitClient(int fd, fd_set *readfds, char fd_array[], int *num_clients)
{
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
		char *buf;
    	g_print("%s",gtk_entry_get_text(GTK_ENTRY(data)));	
    	if (strcmp(gtk_button_get_label ((GtkButton *)widget), "Set name")==0 )
    	{ 
    		buf = (char*)malloc(MSG_SIZE);
    		strcpy(buf,"SETNAME ");strcat(buf,gtk_entry_get_text(GTK_ENTRY(data)));strcat(buf,"\n");
    		write(sockfd,buf,strlen(buf));
    		g_print("%s",msg);
	
    	}
    	//free(buf);

    }

static void button_moi_clicked(GtkWidget *widget,gpointer data)
    {
    	char *buf;
    	buf = (char*)malloc(MSG_SIZE);strcpy(buf,"CHALLENGE ");strcat(buf,gtk_entry_get_text(GTK_ENTRY(data)));strcat(buf,"\n");
    	g_print("%s",buf);
    	write(sockfd,buf,strlen(buf));

    	//notice("Da gui loi moi thach dau!");
    	free(buf);
    }


static void nuoc_di(GtkWidget *widget,gpointer data)
    {	
    	printf("1\n");
    	GtkWidget *button;

    	GtkWidget *button_image_x = gtk_image_new_from_file("icon/x2.png");
    	GtkWidget *button_image_o = gtk_image_new_from_file("icon/o.png");
	//gtk_label_set_text(GTK_LABEL(data),"you clicked button")
    	char pos[4];
    	char* pos2;
    	pos2=(char*)malloc(MSG_SIZE);
    	int i = 0;
    	int x,y,check;
    	strcpy(pos2,"MOVE ");
    	strcat(pos2, gtk_widget_get_name(widget));
		printf("%s\n", pos2);
    	char *saveptr;
    	char *foo, *bar;
    	if (flag == 0)
    	{
    		flag = 1;
    	} else {
    		flag = 0;
    	}
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
		strcat(pos2, "\n");
    	write(sockfd, pos2, strlen(pos2));
		
		foo = strtok_r(pos2, " ", &saveptr);//printf("%d ",atoi(foo));
		bar = strtok_r(NULL, "\n", &saveptr);//printf("%d\n",atoi(bar));

		foo = strtok_r(bar,"-",&saveptr);
		bar = strtok_r(NULL, "\n", &saveptr);
		x=atoi(foo);y=atoi(bar);
		//printf("%d %d\n",x,y);
		free(pos2);
		if (k == 1) {
			gtk_button_set_image ((GtkButton *)widget, button_image_x);
		//g_print("Button is pressed\n");
			gtk_widget_set_sensitive((GtkWidget *)widget,FALSE);
		// if(checkWinning(k, request[0], request[1], komoku))
			// printf("player %i win\n", k);
			komoku[x][y] = 1;
		check = checkWinning(1, x, y, komoku);//printf("%d\n",check);
		if(check == 1){
			write(sockfd,"END\n",4);
			notice_end("YOU WIN!");

			
		}
		

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
			write(sockfd,"END\n",4);
			notice_end("YOU WIN!");
	
			
		}
		//g_print("%d",k);
	}
	/*for (m = 0;m < COLUMN;m++) {
    	for (n = 0;n < ROW;n++) {
    		printf("%d",komoku[m][n]);
    	}
    	printf("\n");
	}*/
	if (check != 1) block_board();

}
void refresh_board(){
	gtk_widget_destroy(window_board);
	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);		
	window_board=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	label=gtk_label_new("Fight!");
	gtk_box_pack_start(GTK_BOX(vbox),label,0,0,0);
	int i,j;
	for(i=0;i<17;i++)
	{
		hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);		
		for(j=0;j<20;j++)
		{
			wid[i][j]=gtk_button_new();
			gchar pos[4];
			sprintf(pos, "%d-%d", i, j);
			gtk_widget_set_name(wid[i][j],pos);
			komoku[i][j]=0;
			
			gtk_widget_set_size_request(wid[i][j],40,40);
			gtk_box_pack_start(GTK_BOX(hbox),wid[i][j],0,1,0);
			
			g_signal_connect(wid[i][j],"clicked",G_CALLBACK(nuoc_di),NULL);
			
		}

		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
	}
	gtk_container_add(GTK_CONTAINER(window_board),vbox);
	gtk_widget_show_all(window_board);
}

static void notice_end(char *text)
{
	
	GtkWidget *window_end=gtk_window_new(GTK_WINDOW_POPUP);
	GtkWidget *dialog,*button;
	dialog = gtk_message_dialog_new ((GtkWindow*)window_end, GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO,GTK_BUTTONS_OK,text);
	
	gtk_dialog_run(dialog);
	gtk_widget_destroy(dialog);
	refresh_board();
}


void block_board(){
	int i,j;
	for(i=0;i<17;i++)
	{	
		for(j=0;j<20;j++)
		{
			
				// gchar pos[4];
			// sprintf(pos, "%d-%d", i, j);
			gtk_widget_set_sensitive(wid[i][j],FALSE);
		}
	}

}
void unblock_board(){
	int i,j;
	for(i=0;i<17;i++)
	{	
		for(j=0;j<20;j++)
		{
			if (komoku[i][j]!=0)
				continue;
				// gchar pos[4];
			// sprintf(pos, "%d-%d", i, j);
			gtk_widget_set_sensitive(wid[i][j],TRUE);
			
		}
	}

}
static void on_accept(GtkWidget* widget,gpointer data)
{

	char* buf;
	buf = (char*)malloc(MSG_SIZE);

	// gdk_threads_leave();
    strcpy(buf,"ACCEPT ");strcat(buf,name);
    printf("%s", buf);
    write(sockfd,buf,strlen(buf));
    free(buf);
    gtk_widget_destroy (GTK_WIDGET (data));
    unblock_board();
    gtk_widget_set_sensitive(button_quit,TRUE);
    gtk_widget_show_all(window_board);

	
 //    gdk_threads_enter();
}
static void on_reject(GtkWidget* widget,gpointer data)
{
	char* buf;
	buf = (char*)malloc(MSG_SIZE);

	// gdk_threads_leave();
    strcpy(buf,"REFUSE ");strcat(buf,name);
    printf("%s", buf);
    write(sockfd,buf,strlen(buf));
    free(buf);
	gtk_widget_destroy (GTK_WIDGET (data));
}


static void menu_event(GtkWidget* widget,gpointer data)
{

}

static void gtk_main_destroy(){
	write(sockfd, "QUIT\n", 5);
	close(sockfd); //close the current client
	exit(0); //end program
}

static void
on_response (GtkDialog *dialog,
             gint       response_id,
             gpointer   data)
{
  /*For demonstration purposes, this will show the int value 
  of the response type*/
  //g_print ("response is %d\n", response_id);
  
  /*This will cause the dialog to be destroyed*/
  gtk_widget_destroy (GTK_WIDGET (dialog));
}

 

static void show_list(GtkWidget* widget,gpointer data)
{

	GtkWindow *window = gtk_window_new(GTK_WINDOW_POPUP);	
  	GtkWidget *dialog;
  	GtkWidget *content_area;
  	GtkWidget *label;
  	GtkWidget *vbox_list;
  	gint response_id;
  	char* buf;

  	buf = (char*)malloc(sizeof MSG_SIZE);
  	strcpy(buf,"LIST\n");
  	write(sockfd,buf,strlen(buf));
  /*Create the dialog window. Modal windows prevent interaction with other 
  windows in the same application*/
  	dialog = gtk_dialog_new_with_buttons ("List user online", 
                                        window, 
                                        GTK_DIALOG_MODAL, 
                                        GTK_STOCK_OK, 
                                        GTK_RESPONSE_OK, 
                                        NULL);

  /*Create a label and attach it to the content area of the dialog*/
  	content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
  //content


  	vbox_list=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  	
  	
  	GtkWidget* view      = gtk_text_view_new();
    GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    GtkTextIter end;

    

    gtk_text_buffer_get_end_iter(buffer, &end);

    gtk_text_buffer_insert(buffer, &end, msg, -1);

  	gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
	gtk_box_pack_start(GTK_BOX(vbox_list),view,0,0,10);	
  	

	 
	 entry_moi=gtk_entry_new();
	 gtk_entry_set_max_length (GTK_ENTRY(entry_moi),50);
	 gtk_entry_set_placeholder_text (GTK_ENTRY(entry_moi),"Nhap ten nguoi choi muon thach dau");
	
	 button_moi=gtk_button_new_with_label("Thach dau");
	 g_signal_connect(button_moi,"clicked",G_CALLBACK(button_moi_clicked), entry_moi);
	 if (t==0) gtk_widget_set_sensitive(button_moi,FALSE);

	hbox_list=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start(GTK_BOX(hbox_list),entry_moi,1,1,10);	
	gtk_box_pack_start(GTK_BOX(hbox_list),button_moi,0,0,10);

	gtk_box_pack_start(GTK_BOX(vbox_list),hbox_list,0,0,10);	

  	gtk_container_add (GTK_CONTAINER (content_area), vbox_list);

  /*The main purpose of this is to show dialog's child widget, label*/
 	gtk_widget_show_all (dialog);
  
  /*Connecting the "response" signal from the user to the associated
  callback function*/
  	g_signal_connect (GTK_DIALOG (dialog), 
                    "response", 
                    G_CALLBACK (on_response), 
                    NULL);

}


static void confirm_dialog ()
{
  	GtkWidget *dialog,*label;
            GtkWidget *content_area;
            GtkWidget *button1,*button2,*hbox;

                    
            gint response_id;

                  
            // dialog = gtk_dialog_new_with_buttons ("Confirm dialog",window,
            //                           GTK_DIALOG_MODAL,
            //                           GTK_STOCK_OK,
            //                           GTK_RESPONSE_OK,
            //                           GTK_STOCK_CANCEL,
            //                           GTK_RESPONSE_CANCEL,

                               
            //                           NULL);
            dialog = gtk_dialog_new();
            button1 = gtk_button_new_with_label("ACCEPT");
            button2 = gtk_button_new_with_label("REJECT");

    
            hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
            gtk_box_pack_start(GTK_BOX(hbox),button1,0,0,10);  
            gtk_box_pack_start(GTK_BOX(hbox),button2,0,0,10);

            g_signal_connect (GTK_BUTTON (button1), 
                     "clicked", 
                     G_CALLBACK (on_accept), 
                     dialog);
            g_signal_connect (GTK_BUTTON (button2), 
                     "clicked", 
                     G_CALLBACK (on_reject), 
                     dialog);


            
            /*Create a label and attach it to the content area of the dialog*/
            content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
                      //content
            label = gtk_label_new ("Ban nhan duoc mot loi thach dau");
            gtk_container_add (GTK_CONTAINER (content_area),label );
            gtk_container_add (GTK_CONTAINER (content_area),hbox ); 
                      /*The main purpose of this is to show dialog's child widget, label*/
            gtk_widget_show_all (dialog);
                      
                      /*Connecting the "response" signal from the user to the associated
                      callback function*/ 
            g_signal_connect (GTK_DIALOG (dialog), "response", 
                                        G_CALLBACK (on_response), 
                                        NULL);
}

static void roi_phong(GtkWidget *widget,gpointer data){
	//TODO

	write(sockfd,"EXIT\n",5);
	k=1;
	gtk_widget_set_sensitive(button_quit,FALSE);
	notice("Ban da roi khoi phong choi");

	refresh_board();
	gtk_widget_hide (window_board);
}



   //  gtk_widget_destroy(dialog);

void initBoard(int argc, char*argv[]){

	int i,j;

	gtk_init(&argc,&argv);
	
	
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	window_board=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window,500,500);
	gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	
	

	
	
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
	vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	label=gtk_label_new("Fight!");
	gtk_box_pack_start(GTK_BOX(vbox),label,0,0,0);
	for(i=0;i<17;i++)
	{
		hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);		
		for(j=0;j<20;j++)
		{
			gchar pos[4];
			sprintf(pos, "%d-%d", i, j);
			wid[i][j]=gtk_button_new();
			gtk_widget_set_name(wid[i][j],pos);

			gtk_widget_set_size_request(wid[i][j],40,40);
			gtk_box_pack_start(GTK_BOX(hbox),wid[i][j],0,1,0);
			
			g_signal_connect(wid[i][j],"clicked",G_CALLBACK(nuoc_di),NULL);
			
		}

		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
	}
	block_board();
	



//Menu//image
	image=gtk_image_new_from_file("icon/logo1.png");
	gtk_image_set_pixel_size (GTK_IMAGE(image),70);

//Nhap user va connect
//entry
	entry=gtk_entry_new();
	gtk_entry_set_max_length (GTK_ENTRY(entry),50);
	gtk_entry_set_placeholder_text (GTK_ENTRY(entry),"Nhap username vao day");
	button_connect=gtk_button_new_with_label("Set name");
	g_signal_connect(button_connect,"clicked",G_CALLBACK(button_connect_clicked), entry);

	hbox2=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_pack_start(GTK_BOX(hbox2),entry,1,1,10);	
	gtk_box_pack_start(GTK_BOX(hbox2),button_connect,0,0,10);



//Danh sach online
	GtkWidget *vbox2,*hbox3,*radio,*radio2,*label1,*button_invite;
	vbox2=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

	//list
	GtkWidget *buttonList;
	buttonList = gtk_button_new_with_label("List user online");
	g_signal_connect(buttonList,"clicked",G_CALLBACK(show_list),NULL);


	button_quit = gtk_button_new_with_label("Roi phong");
	g_signal_connect(button_quit,"clicked",G_CALLBACK(roi_phong),NULL);
	gtk_widget_set_sensitive(button_quit,FALSE);
	
// 	label = gtk_label_new("Nguoi choi dang online");
// 	gtk_box_pack_start(GTK_BOX(vbox2),label,0,0,0);
// 	radio=gtk_radio_button_new_with_label(NULL,"User 1");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio,0,0,0);


// 	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 2");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
// 	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 3");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
// 	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 4");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
// 	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 5");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
// 	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 6");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
// 	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 7");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
// 	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 8");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
// 	radio2=gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio)),"User 9");
// 	gtk_box_pack_start(GTK_BOX(vbox2),radio2,0,0,0);
// //thach dau

	// button_invite=gtk_button_new_with_label("Thach dau!");
	// //gtk_button_set_alignment (GTK_BUTTON(button_invite),0.5,0.5);
	// g_signal_connect(button_invite,"clicked",G_CALLBACK(button_invite_clicked), NULL);
	// hbox3=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	//gtk_box_pack_start(GTK_BOX(hbox3),button_invite,1,1,10);
	
//producer

	GtkWidget *header;
	header = gtk_header_bar_new();
	gtk_header_bar_set_title (GTK_HEADER_BAR (header),"Nhom 9");
	gtk_header_bar_set_subtitle (GTK_HEADER_BAR (header),"Phan Bao Hung, Le Minh Duc, Nguyen Duc Toan");	
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header),
		TRUE);



	label=gtk_label_new("\n\n____Nhom thuc hien____\n          Phan Bao Hung\n        Nguyen Duc Toan    \n            Le Minh Duc     ");
	
//box pack
	vbox1=gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
	//gtk_box_pack_start(GTK_BOX(vbox1),header,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),image,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),hbox2,0,0,10);
	gtk_box_pack_start(GTK_BOX(vbox1),buttonList,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox1),button_quit,0,0,0);
	gtk_box_pack_start(GTK_BOX(vbox1),label,0,0,0);
	//gtk_widget_set_size_request(vbox,600,400);
	//gtk_widget_set_size_request(vbox,600,400);
	

	//hbox_lon=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	
	// gtk_box_pack_start(GTK_BOX(hbox_lon),vbox1,0,0,0);
	// gtk_box_pack_start(GTK_BOX(hbox_lon),vbox,0,0,0);
	
	g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_destroy),NULL);
	gtk_container_add(GTK_CONTAINER(window),vbox1);
	gtk_container_add(GTK_CONTAINER(window_board),vbox);
	gtk_widget_show_all(window);
	//g_signal_connect(window_board,"destroy",G_CALLBACK(roi_phong),NULL);
	//gtk_widget_show_all(window_board);


}

void updateGui(int x, int y){

    	GtkWidget *button_image_x = gtk_image_new_from_file("icon/x2.png");
    	GtkWidget *button_image_o = gtk_image_new_from_file("icon/o.png");
	//gtk_label_set_text(GTK_LABEL(data),"you clicked button")
    	
    	if (k==1) 
    		komoku[x][y] = 2;
    	else komoku[x][y] = 1;
		if (k == 1) {
			gtk_button_set_image (wid[x][y], button_image_o);
		//g_print("Button is pressed\n");
			gtk_widget_set_sensitive(wid[x][y],FALSE);
		// if(checkWinning(k, request[0], request[1], komoku))
			// printf("player %i win\n", k);
		
		}

		//g_print("%d",k);	
	else {
		gtk_button_set_image ((GtkWidget *)wid[x][y], button_image_x);
		//g_print("Button is pressed\n");
		gtk_widget_set_sensitive((GtkWidget *)wid[x][y],FALSE);
		// if(checkWinning(k, request[0], request[1], komoku))
			// printf("player %i win\n", k);	
	}
	unblock_board();
}
static void receivedCmd(){

	while(1){

		gdk_threads_enter();
		testfds=clientfds;
		gdk_threads_leave();
		select(FD_SETSIZE,&testfds,NULL,NULL,NULL);
		//printf("1\n");
		for(fd=0;fd<FD_SETSIZE;fd++){
			if(FD_ISSET(fd,&testfds)){

				if(fd==sockfd){   /*Accept data from open socket */
					//printf("client - read\n");

					//read data from open socket
					result = read(sockfd, msg, MSG_SIZE);
					msg[result] = '\0';  /* Terminate string with null */
					buffer1 = (char*)malloc(MSG_SIZE);
					strcpy(buffer1,msg);
					char* buffer = strtok(buffer1, " ");
					if (!strcmp(buffer,"QUIT")) {
						printf("%s", msg +5);                   
						close(sockfd);
						exit(0);
					}
					else if (strncmp(msg,"CHALLENGED",10)==0) {
						name = strtok(msg, " ");
		      			name = strtok(NULL, " ");
						//printf("%s\n",name);
						// dialog voi thong bao va 2 nut accept, refuse
						gdk_threads_enter();
						k=2;
						confirm_dialog();
						gdk_threads_leave();
					}
					else if (strncmp(msg,"ACCEPTED",8)==0) {
					 	gdk_threads_enter();
					 	gtk_widget_set_sensitive(button_quit,TRUE);
					 	gtk_widget_show_all(window_board);
					 	gdk_threads_leave();
					// 
					}
					else if (strncmp(msg,"MOVE",4)==0) {
						int x,y;
						char* saveptr;
						char* foo,*bar;
						foo = strtok_r(msg, " ", &saveptr);//printf("%d ",atoi(foo));
						bar = strtok_r(NULL, "\n", &saveptr);//printf("%d\n",atoi(bar));

						
						foo = strtok_r(bar,"-",&saveptr);
						bar = strtok_r(NULL, "\n", &saveptr);
						x=atoi(foo);y=atoi(bar);
						gdk_threads_enter();
						updateGui(x,y);
						
						gdk_threads_leave();

					}   
					else if (strncmp(msg,"OK",2)==0) {
						gdk_threads_enter();
						notice("Set name success !");
    					gtk_widget_set_sensitive (button_connect,FALSE);
    					t=1;
    					gtk_widget_set_sensitive (entry,FALSE);
    					gdk_threads_leave();     		
    		 		}
					else if (strncmp(msg,"SETFAIL",7)==0) {
						gdk_threads_enter();

						notice("Set name fail !");
						
						gdk_threads_leave();
					}
					else if (strncmp(msg,"EXIT",4)==0) {
						gdk_threads_enter();

						notice("Opponent has left room");
						refresh_board();
						gtk_widget_hide(window_board);
						gtk_widget_set_sensitive(button_quit,FALSE);
						gdk_threads_leave();
					}
					else if (strncmp(msg,"END",3)==0) {
						gdk_threads_enter();

						notice_end("YOU LOSE!");
						block_board();
						
						gdk_threads_leave();

					}
					else if (strncmp(msg,"INVALID",7)==0) {
						gdk_threads_enter();

						notice("INVALID NAME!");
						
						gdk_threads_leave();

					}
					else if (strncmp(msg,"UNAVAILABLE",11)==0) {
						gdk_threads_enter();

						notice("USER CURRENTLY UNAVAILABLE!");
						
						gdk_threads_leave();

					}
					else {
						printf("Message from server: %s", msg);
						// memset(msg, 0, MSG_SIZE);
					}
					
				}
				else if(fd == 0){ /*process keyboard activiy*/
					// printf("client - send\n");

					fgets(kb_msg, MSG_SIZE+1, stdin);
					//printf("%s\n",kb_msg);
					if (strcmp(kb_msg, "QUIT\n")==0) {
						//sprintf(msg, "QUIT Client is shutting down.\n");
						write(sockfd, kb_msg, strlen(kb_msg));
						close(sockfd); //close the current client
						exit(0); //end program
					}
					else {
						/* sprintf(kb_msg,"%s",alias);
						msg[result]='\0';
						strcat(kb_msg,msg+1);*/

						//sprintf(kb_msg, "%s", msg);
						write(sockfd, kb_msg, strlen(kb_msg));
					}
					//gdk_threads_leave();                                                 
				}
				//gdk_threads_leave();          
			} else {
				//gdk_threads_leave();
			}
		}
		//gdk_threads_leave();
	}
}

int main (int argc, char* argv[])
{	
	if(argc==2 || argc==4){
		if( ! g_thread_supported() )
			g_thread_init( NULL );

		gdk_threads_init();

		gdk_threads_enter();

		if(!strcmp("-p",argv[1])){
			if(argc==2){
				printf("Invalid parameters.\nUsage: chat [-p PORT] HOSTNAME\n");
				exit(0);
			}else{
				sscanf(argv[2],"%i",&port);
				strcpy(hostname,argv[3]);
			}
		}else{
			port=MYPORT;
			strcpy(hostname,argv[1]);
		}
		printf("\n*** Client program starting (enter \"quit\" to stop): \n");
		fflush(stdout);

/* Create a socket for the client */
		sockfd = socket(AF_INET, SOCK_STREAM, 0);

/* Name the socket, as agreed with the server */
		hostinfo = gethostbyname(hostname);  /* look for host's name */
		address.sin_addr = *(struct in_addr *)*hostinfo -> h_addr_list;
		address.sin_family = AF_INET;
		address.sin_port = htons(port);

/* Connect the socket to the server's socket */
		if(connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0){
			perror("connecting");
			exit(1);
		}

		fflush(stdout);

		FD_ZERO(&clientfds);
		FD_SET(sockfd,&clientfds);
		FD_SET(0,&clientfds);//stdin

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
		thread = g_thread_create(receivedCmd, NULL, FALSE, &error);
		if (!thread)
		{
			g_print("Error: %s\n", error->message);
			return (-1);
		}

		gtk_main();

		gdk_threads_leave();

//Client code


		return 0;

	}
}