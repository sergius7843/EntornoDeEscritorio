//WallpaperWindow.cpp
#include "WallpaperWindow.hpp"
#include <iostream>

WallpaperWindow::WallpaperWindow(const std::string& wallpaper_path) 
    : current_wallpaper(wallpaper_path){

    set_decorated(false); // sin borde ni botones
    // Añadir clase CSS personalizada
    get_style_context()->add_class("wallpaper-window");

    set_resizable(false);
    set_title("Fondo de escritorio");
    fullscreen();

    // Configura el CSS para el fondo y los botones con el archivo JSON
    ThemeManager theme("config/theme.json");
    auto style_context = get_style_context();
    style_context->add_provider(theme.get_css_provider(), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    try {
        image.set_filename(current_wallpaper);
        image.set_content_fit(Gtk::ContentFit::COVER);
    }
    catch (const Glib::Error& ex) {
        std::cerr << "Error loading wallpaper: " << ex.what() << std::endl;
    }
    
    set_child(image);
}

void WallpaperWindow::apply_theme(ThemeManager* theme) {
    auto context = get_style_context();
    
    // Eliminar proveedor anterior si existe
    if (current_provider) {
        context->remove_provider(current_provider);
    }
    
    current_provider = theme->get_css_provider();
    context->add_provider(current_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}