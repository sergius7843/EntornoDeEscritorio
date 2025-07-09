// CoreSystem.cpp
#include "CoreSystem.hpp"
#include <iostream>
#include "EventManager.hpp"

CoreSystem::CoreSystem(const std::string& theme_path) 
    : wallpaper(nullptr), top_panel(nullptr), app_launcher(nullptr), theme(nullptr), 
      theme_path(theme_path), context_menu(nullptr) {}

CoreSystem::~CoreSystem() {
    stop();
}

void CoreSystem::start(Glib::RefPtr<Gtk::Application> app) {
    this->app = app;
    theme = new ThemeManager(theme_path);
    
    // Crear componentes
    wallpaper = new WallpaperWindow("assets/wallpaper/wallpaperUno.jpg");
    top_panel = new TopPanel();
    app_launcher = new AppLauncher(); // Nuevo

    // Crear menu contextual
    context_menu = new DesktopContextMenu();
    //app->add_window(*context_menu);

    // IMPORTANTE: Establecer el wallpaper como padre del popover
    context_menu->set_parent(*wallpaper);

    // Inyectar dependencias
    top_panel->set_app_launcher(app_launcher);
    
    // Aplicar tema
    wallpaper->apply_theme(theme);
    top_panel->apply_theme(theme);
    app_launcher->apply_theme(theme); // Nuevo
    
    // Registrar ventanas
    app->add_window(*wallpaper);
    app->add_window(*top_panel);
    app->add_window(*app_launcher); // Registrar AppLauncher
    
    // Mostrar componentes
    wallpaper->show();
    top_panel->show();
    app_launcher->hide(); // Inicialmente oculto

    // Configurar menu contextual despues de que todo este inicializado
    setup_context_menu();
}

void CoreSystem::stop() {
    if (top_panel) {
        // No eliminar app_launcher aquí, CoreSystem es el dueño
        delete top_panel;
        top_panel = nullptr;
    }
    if (wallpaper) {
        delete wallpaper;
        wallpaper = nullptr;
    }
    if (app_launcher) { // Nuevo
        delete app_launcher;
        app_launcher = nullptr;
    }
    if (context_menu) {
        delete context_menu;
        context_menu = nullptr;
    }
    if(theme) {
        delete theme;
        theme = nullptr;
    }
}

void CoreSystem::reload_theme() {
    if (theme) {
        theme->reload();
        wallpaper->apply_theme(theme);
        top_panel->apply_theme(theme);
        app_launcher->apply_theme(theme); // Nuevo
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