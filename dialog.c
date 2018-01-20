#include <gtk/gtk.h>
gtk_widget_set_sensitive((GtkWidget *)widget,FALSE);
static void
on_response (GtkDialog *dialog,
             gint       response_id,
             gpointer   user_data)
{
  /*For demonstration purposes, this will show the int value 
  of the response type*/
  g_print ("response is %d\n", response_id);
  
  /*This will cause the dialog to be destroyed*/
  gtk_widget_destroy (GTK_WIDGET (dialog));
}
static void
lock_click (GtkButton *button,
             gpointer   user_data){
  gtk_widget_set_sensitive((GtkWidget *)widget,FALSE);
}


/*Callback function in which reacts to the "clicked" signal*/
static void
show_dialog (GtkButton *button,
             gpointer   user_data)
{
  GtkWindow *window = user_data; 
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

            g_signal_connect (GTK_BUTTON (button1), 
                     "clicked", 
                     G_CALLBACK (lock_click), 
                     GTK_WINDOW (window));
            hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
            gtk_box_pack_start(GTK_BOX(hbox),button1,0,0,10);  
            gtk_box_pack_start(GTK_BOX(hbox),button2,0,0,10);




            
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



static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
 
  /*Create a window with a title and a default size*/
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "GNOME Button");
  gtk_window_set_default_size (GTK_WINDOW (window), 250, 50);

  /*Create a button with a label, and add it to the window*/
  button = gtk_button_new_with_label ("Click Me");
  gtk_container_add (GTK_CONTAINER (window), button);
 
  /*Connecting the clicked signal to the callback*/
  g_signal_connect (GTK_BUTTON (button), 
                    "clicked", 
                    G_CALLBACK (show_dialog), 
                    GTK_WINDOW (window));
 
  gtk_widget_show_all (window);
}
 


// int
// main (int argc, char **argv)
// {
//   GtkApplication *app;
//   int status;
 
//   app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
//   g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
//   status = g_application_run (G_APPLICATION (app), argc, argv);
//   g_object_unref (app);
 
//   return status;
// }

void main (int argc, char **argv)
 { 
  gtk_init(&argc,&argv);
  GtkWidget *window, *button;
  
  window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(window,1000,1000);
  gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
 
  button = gtk_button_new_with_label ("Click Me");
  gtk_container_add (GTK_CONTAINER (window), button);
 
  /*Connecting the clicked signal to the callback*/
  g_signal_connect (GTK_BUTTON (button), 
                    "clicked", 
                    G_CALLBACK (show_dialog), 
                    GTK_WINDOW (window));
  gtk_widget_show_all (window);
  gtk_main();
}