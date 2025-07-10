// CoreSystem.cpp
#include "CoreSystem.hpp"
#include <iostream>
#include "EventManager.hpp"

CoreSystem::CoreSystem(const std::string& theme_path) 
    : theme_path(theme_path) {}

CoreSystem::~CoreSystem() {
    stop();
}

void CoreSystem::start(Glib::RefPtr<Gtk::Application> app) {
    this->app = app;
    theme = std::make_unique<ThemeManager>(theme_path);
    
    wallpaper = std::make_unique<WallpaperWindow>("assets/wallpaper/wallpaperUno.jpg");
    top_panel = std::make_unique<TopPanel>();
    app_launcher = std::make_unique<AppLauncher>();
    context_menu = std::make_unique<DesktopContextMenu>();

    context_menu->set_parent(*wallpaper);
    top_panel->set_app_launcher(app_launcher.get());
    
    // CORRECCIÓN: Usar theme.get() en lugar de theme
    wallpaper->apply_theme(theme.get());
    top_panel->apply_theme(theme.get());
    app_launcher->apply_theme(theme.get());
    
    app->add_window(*wallpaper);
    app->add_window(*top_panel);
    app->add_window(*app_launcher);
    
    wallpaper->show();
    top_panel->show();
    app_launcher->hide();

    setup_context_menu();
}

void CoreSystem::stop() {
    if(app) {
        if(top_panel) app->remove_window(*top_panel);
        if(wallpaper) app->remove_window(*wallpaper);
        if(app_launcher) app->remove_window(*app_launcher);
    }

    context_menu.reset();
    app_launcher.reset();
    top_panel.reset();
    wallpaper.reset();
    theme.reset();
}

void CoreSystem::reload_theme() {
    if (theme) {
        theme->reload();
        // CORRECCIÓN: Usar theme.get() en lugar de theme
        wallpaper->apply_theme(theme.get());
        top_panel->apply_theme(theme.get());
        app_launcher->apply_theme(theme.get());
    }
}

void CoreSystem::setup_context_menu() {
    // Registrar evento de clic derecho
    EventManager::get_instance().register_event("desktop_right_click", [this]() {
        std::cout << "Evento de clic derecho recibido" << std::endl;
        
        // Posición por defecto (centro de la pantalla)
        double x = 200;
        double y = 200;
        
        // Intentar obtener posición real del cursor
        auto display = Gdk::Display::get_default();
        if (display) {
            auto seat = display->get_default_seat();
            if (seat) {
                auto pointer = seat->get_pointer();
                if (pointer) {
                    double cursor_x, cursor_y;
                    auto surface = pointer->get_surface_at_position(cursor_x, cursor_y);
                    if (surface) {
                        x = cursor_x;
                        y = cursor_y;
                        std::cout << "Posición del cursor: " << x << ", " << y << std::endl;
                    }
                }
            }
        }
        
        // Mostrar menú en posición calculada
        context_menu->show_at_position(x, y);
    });

    // Añadir items al menú
    context_menu->add_item({
        "Actualizar",
        [this]() {
            std::cout << "Ejecutando acción de actualización" << std::endl;
            if (wallpaper) {
                wallpaper->refresh_desktop();
            }
        },
        "view-refresh-symbolic"
    });
    
    context_menu->add_item({
        "Cambiar fondo",
        []() { 
            std::cout << "Cambiar fondo" << std::endl; 
        },
        "preferences-desktop-wallpaper-symbolic"
    });
    
    context_menu->add_item({
        "Configuración",
        []() { 
            std::cout << "Abrir configuración" << std::endl; 
        },
        "preferences-system-symbolic"
    });
}