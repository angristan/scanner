#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <string.h>
#include "functions.h"
#include <tgmath.h>

int main(int argc, char **argv) {

    GtkWidget *cartResume = NULL;
    GtkBuilder *builder = NULL;
    GtkButton *validateButton = NULL;
    GtkButton *showCart = NULL;
    GtkButton *returnCart = NULL;
    GtkButton *addToCart = NULL;
    GtkEntry *barcode = NULL;
    GtkEntry *quantity = NULL;
    GError *error = NULL;
    AppliStruct *appliStruct = NULL;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../gui.glade", &error);

    appliStruct = InitStruct(appliStruct, builder);

    if (error) {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free(error);
        return code;
    }

    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    GFile *css_file = g_file_new_for_path("../css/global.css");
    gtk_css_provider_load_from_file(provider, css_file, &error);
    g_object_unref(css_file);

    gtk_builder_connect_signals(builder, NULL);

    validateButton = GTK_BUTTON(gtk_builder_get_object(builder, "validateButton"));
    showCart = GTK_BUTTON(gtk_builder_get_object(builder, "cartButton"));
    cartResume = GTK_WIDGET(gtk_builder_get_object(builder, "cartWindow"));
    returnCart = GTK_BUTTON(gtk_builder_get_object(builder, "returncartButton"));
    addToCart = GTK_BUTTON(gtk_builder_get_object(builder, "addcartButton"));
    barcode = GTK_ENTRY(gtk_builder_get_object(builder, "barrecodeEntry"));
    quantity = GTK_ENTRY(gtk_builder_get_object(builder, "quantityEntry"));

    g_signal_connect(validateButton, "clicked", (GCallback) GetLog, appliStruct);
    g_signal_connect(appliStruct->mainWindow, "destroy", (GCallback) OnDestroy, NULL);
    g_signal_connect(appliStruct->scanproduct, "destroy", (GCallback) OnDestroy, NULL);
    g_signal_connect(showCart, "clicked", (GCallback) OpenCart, builder);
    g_signal_connect(cartResume, "destroy", (GCallback) OnDestroy, NULL);
    g_signal_connect(returnCart, "clicked", (GCallback) ReturnCart, builder);
    g_signal_connect(addToCart, "clicked", (GCallback) GetBarCode, barcode);
    g_signal_connect(addToCart, "clicked", (GCallback) GetQuantity, quantity);

    gtk_widget_show_all(appliStruct->mainWindow);

    gtk_main();

    return EXIT_SUCCESS;
}
