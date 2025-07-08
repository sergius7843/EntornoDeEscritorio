// TopPanel.cpp
#include "TopPanel.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <gdkmm/monitor.h>
#include <glibmm/refptr.h>
#include <gdkmm/display.h>
#include "../config/ThemeManager.hpp"
#include "../app_launcher/AppLauncher.hpp"  

// para aplicar los temas
#include "../config/ThemeManager.hpp"


TopPanel::TopPanel() : box(Gtk::Orientation::HORIZONTAL) {
    set_decorated(false);
    set_resizable(false);
    set_title("Panel Superior");
    
    // Obtener el tamaño de la pantalla
    auto display = Gdk::Display::get_default();
    if (display) {
        // Obtener la lista de monitores
        auto monitors = display->get_monitors();
        if (monitors && monitors->get_n_items() > 0) {
            // Obtener el primer monitor usando get_object()
            auto monitor_obj = monitors->get_object(0);
            auto monitor = std::dynamic_pointer_cast<Gdk::Monitor>(monitor_obj);
            if (monitor) {
                // CORRECCIÓN: get_geometry() requiere un parámetro Gdk::Rectangle
                Gdk::Rectangle geometry;
                monitor->get_geometry(geometry);
                set_default_size(geometry.get_width(), 30);
            } else {
                set_default_size(800, 30);
            }
        } else {
            // Fallback si no hay monitores
            set_default_size(800, 30);
        }
    } else {
        // Fallback si no hay display
        set_default_size(800, 30);
    }
    
    // Configurar CSS para el fondo negro
    /* 
    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_data(
    "window { background-color: #000000; }"
    "label { color: #ffffff; }"
    "button { background-color: #333333; color: #ffffff; border: none; }"
    "button:hover { background-color: #555555; }"
    );
    */

    
    // aplicando el tema
    ThemeManager theme("config/theme.json");
    auto style_context = get_style_context();
    style_context->add_provider(theme.get_css_provider(), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    //auto style_context = get_style_context();
    //style_context->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    // Configurar botón de menú
    menu_button.set_label("☰");
    menu_button.set_margin_end(10);

    // app_launcher = new AppLauncher();

    menu_button.signal_clicked().connect([this]() {
        if (app_launcher) {
            app_launcher->toggle_visibility();
        }
    });
    
    // Configurar reloj
    clock.set_margin_start(10);
    clock.set_hexpand(true);
    clock.set_halign(Gtk::Align::END);
    update_time();
    
    // Agregar elementos al box
    box.append(menu_button);
    box.append(clock);
    
    // ¡IMPORTANTE! Agregar el box a la ventana (esto faltaba en tu código original)
    set_child(box);
    
    // Configurar el timer para actualizar la hora cada segundo
    timer_connection = Glib::signal_timeout().connect_seconds(
        sigc::mem_fun(*this, &TopPanel::update_time), 1
    );
}

TopPanel::~TopPanel() {
    // delete app_launcher;
}

void TopPanel::set_app_launcher(AppLauncher* launcher) {
    app_launcher = launcher;
}

void TopPanel::apply_theme(ThemeManager* theme) {
    auto context = get_style_context();
    
    // Limpiar proveedor anterior si existe
    if (current_provider) {
        context->remove_provider(current_provider);
    }
    
    context->add_provider(theme->get_css_provider(), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

bool TopPanel::update_time() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(now, "%H:%M:%S");
    clock.set_text(oss.str());
    return true;
}
/*
void WallpaperWindow::apply_theme(ThemeManager* theme) {
    auto context = get_style_context();
    context->add_provider(theme->get_css_provider(), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
*/
