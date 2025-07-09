// src/core/CoreSystem.hpp
#pragma once
#include "../wallpaper/WallpaperWindow.hpp"
#include "../panel/TopPanel.hpp"
#include <gtkmm/application.h>
#include "../config/ThemeManager.hpp"
#include "../app_launcher/AppLauncher.hpp"
#include "../context_menu/DesktopContextMenu.hpp"

class CoreSystem {
public:
    CoreSystem(const std::string& theme_path = "config/theme.json");
    ~CoreSystem();

    void start(Glib::RefPtr<Gtk::Application> app);  // Cambiando la firma
    void stop();   // Cierra todos los componentes

    void reload_theme();

    void setup_context_menu();

private:
    WallpaperWindow* wallpaper;
    TopPanel* top_panel;
    AppLauncher* app_launcher; // Nuevo
    Glib::RefPtr<Gtk::Application> app;
    ThemeManager* theme;    // Tema global compartido
    std::string theme_path;

    DesktopContextMenu* context_menu; // Nuevo
};
