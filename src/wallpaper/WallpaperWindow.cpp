//WallpaperWindow.cpp
#include "WallpaperWindow.hpp"
#include "../core/EventManager.hpp"
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

    // Configurar eventos después de que todo esté inicializado
    setup_event_listeners();
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

void WallpaperWindow::on_right_click_pressed(int n_press, double x, double y) {
    std::cout << "Clic derecho detectado en posición: " << x << ", " << y << std::endl;
    
    // Disparar evento global de clic derecho
    EventManager::get_instance().trigger_event("desktop_right_click");
}

void WallpaperWindow::refresh_desktop() {
    // 1. Crear efecto de parpadeo
    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_data(".wallpaper-window { opacity: 0.5; }");
    
    // Aplicar efecto temporal
    auto style_context = get_style_context();
    style_context->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    // 2. Forzar redibujado
    queue_draw();
    
    // 3. Restaurar estado original después de un breve retraso
    Glib::signal_timeout().connect_once([this, css_provider, style_context]() {
        style_context->remove_provider(css_provider);
        queue_draw();
    }, 100);
}

Gtk::Widget* WallpaperWindow::get_root_widget() {
    return &image; // o `this` si prefieres usar toda la ventana como ancla
}

void WallpaperWindow::setup_event_listeners() {
    // Configurar gesto de clic derecho
    right_click_gesture = Gtk::GestureClick::create();
    right_click_gesture->set_button(GDK_BUTTON_SECONDARY); // Botón derecho
    right_click_gesture->signal_pressed().connect(
        sigc::mem_fun(*this, &WallpaperWindow::on_right_click_pressed));
    
    // Agregar el controlador al widget raíz (la ventana)
    add_controller(right_click_gesture);
}
