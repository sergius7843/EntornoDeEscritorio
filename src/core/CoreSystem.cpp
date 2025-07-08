// CoreSystem.cpp
#include "CoreSystem.hpp"

CoreSystem::CoreSystem(const std::string& theme_path) 
    : wallpaper(nullptr), top_panel(nullptr), app_launcher(nullptr), theme(nullptr), 
      theme_path(theme_path) {}

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
    if (theme) {
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