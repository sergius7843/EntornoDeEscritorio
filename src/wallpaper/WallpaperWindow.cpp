//WallpaperWindow.cpp
#include "WallpaperWindow.hpp"
#include "../core/EventManager.hpp"
#include <iostream>


WallpaperWindow::WallpaperWindow(const std::string& wallpaper_path) 
    : current_wallpaper(wallpaper_path) {

    set_decorated(false);
    get_style_context()->add_class("wallpaper-window");
    set_resizable(false);
    set_title("Fondo de escritorio");
    fullscreen();

    // Eliminado: ThemeManager local redundante
    // El tema se aplicará externamente via apply_theme()
    
    try {
        image.set_filename(current_wallpaper);
        image.set_content_fit(Gtk::ContentFit::COVER);
    }
    catch (const Glib::Error& ex) {
        std::cerr << "Error loading wallpaper: " << ex.what() << std::endl;
    }
    
    set_child(image);
    setup_event_listeners();
}

WallpaperWindow::~WallpaperWindow() {
    // Desconectar todas las señales y gestos
    if (right_click_gesture) {
        remove_controller(right_click_gesture);
        right_click_gesture.reset();
    }
    
}

void WallpaperWindow::apply_theme(ThemeManager* theme) {
    auto context = get_style_context();
    
    // Limpiar proveedor anterior
    if (current_provider) {
        context->remove_provider(current_provider);
    }
    
    // Aplicar nuevo proveedor
    current_provider = theme->get_css_provider();
    context->add_provider(current_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void WallpaperWindow::on_right_click_pressed(int n_press, double x, double y) {
    std::cout << "Clic derecho detectado en posición: " << x << ", " << y << std::endl;
    EventManager::get_instance().trigger_event("desktop_right_click");
}

void WallpaperWindow::refresh_desktop() {
    // Crear efecto de parpadeo
    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_data(".wallpaper-window { opacity: 0.5; }");
    
    auto style_context = get_style_context();
    style_context->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    queue_draw();

    // Guardamos la conexión para poder desconectarla si hace falta
    Glib::signal_timeout().connect_once(
        [this, css_provider]() {
            this->get_style_context()->remove_provider(css_provider);
            this->queue_draw();
        },
        100
    );
}

void WallpaperWindow::setup_event_listeners() {
    right_click_gesture = Gtk::GestureClick::create();
    right_click_gesture->set_button(GDK_BUTTON_SECONDARY);
    right_click_gesture->signal_pressed().connect(
        sigc::mem_fun(*this, &WallpaperWindow::on_right_click_pressed));
    
    add_controller(right_click_gesture);
}