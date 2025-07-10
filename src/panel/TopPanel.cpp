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
        auto monitors = display->get_monitors();
        if (monitors && monitors->get_n_items() > 0) {
            auto monitor_obj = monitors->get_object(0);
            auto monitor = std::dynamic_pointer_cast<Gdk::Monitor>(monitor_obj);
            if (monitor) {
                Gdk::Rectangle geometry;
                monitor->get_geometry(geometry);
                set_default_size(geometry.get_width(), 30);
            } else {
                set_default_size(800, 30); // Fallback
            }
        } else {
            set_default_size(800, 30); // Fallback
        }
    } else {
        set_default_size(800, 30); // Fallback
    }
    
    // Eliminado: Creación redundante de ThemeManager
    // El tema se aplicará externamente via apply_theme()
    
    // Configurar botón de menú
    menu_button.set_label("☰");
    menu_button.set_margin_end(10);

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
    
    set_child(box);
    
    // Configurar timer
    timer_connection = Glib::signal_timeout().connect_seconds(
        sigc::mem_fun(*this, &TopPanel::update_time), 1
    );
}

TopPanel::~TopPanel() {
    // Importante: Desconectar señal del timer
    timer_connection.disconnect();
}

void TopPanel::set_app_launcher(AppLauncher* launcher) {
    app_launcher = launcher;
}

void TopPanel::apply_theme(ThemeManager* theme) {
    auto context = get_style_context();
    
    // Limpiar proveedor anterior
    if (current_provider) {
        context->remove_provider(current_provider);
    }
    
    // Aplicar nuevo proveedor
    current_provider = theme->get_css_provider();
    context->add_provider(current_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    // Aplicar clases específicas
    context->add_class("top-panel");
}

bool TopPanel::update_time() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
    localtime_r(&in_time_t, &tm_buf); // Versión segura
    
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%H:%M:%S");
    clock.set_text(oss.str());
    return true; // Mantener activo
}