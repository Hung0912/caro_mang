#include <gtk/gtk.h>

static gpointer
thread_func( gpointer data )
{
    while( TRUE )
    {
        gdk_threads_enter();
        gtk_button_set_label( GTK_BUTTON( data ), "Thread" );
        gdk_threads_leave();
        printf("1\n");
    }

    return( NULL );
}

static void
cb_clicked( GtkButton *button,
            gpointer   data )
{
    /* No need to call gdk_threads_enter/gdk_threads_leave,
       since gtk callbacks are executed withing main lock. */
    gtk_button_set_label( button, "Clicked" );
}

static gboolean
cb_timeout( gpointer data )
{
    /* Timeouts also don't automatically obtain main lock, but
       since we added this one using gdk_threads_add_timeout,
       lock has been obtained for us. */
    gtk_button_set_label( GTK_BUTTON( data ), "Timeout" );

    return( TRUE );
}

int
main( int    argc,
      char **argv )
{
    GtkWidget *window;
    GtkWidget *button;
    GThread   *thread;
    GError    *error = NULL;

    /* Secure glib */
    if( ! g_thread_supported() )
        g_thread_init( NULL );

    /* Secure gtk */
    gdk_threads_init();

    /* Obtain gtk's global lock */
    gdk_threads_enter();

    /* Do stuff as usual */
    gtk_init( &argc, &argv );

    window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    g_signal_connect( G_OBJECT( window ), "destroy",
                      G_CALLBACK( gtk_main_quit ), NULL );

    button = gtk_button_new_with_label( "Initial value" );
    g_signal_connect( G_OBJECT( button ), "clicked",
                      G_CALLBACK( cb_clicked ), NULL );
    gtk_container_add( GTK_CONTAINER( window ), button );

    //gdk_threads_add_timeout( 2000, cb_timeout, (gpointer)button );

    /* Create new thread */
    thread = g_thread_create( thread_func, (gpointer)button,
                              FALSE, &error );
    if( ! thread )
    {
        g_print( "Error: %s\n", error->message );
        return( -1 );
    }

    gtk_widget_show_all( window );

    gtk_main();

    /* Release gtk's global lock */
    gdk_threads_leave();

    return( 0 );
}