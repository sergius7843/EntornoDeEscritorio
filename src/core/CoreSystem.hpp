// src/core/CoreSystem.hpp
#pragma once
#include "../wallpaper/WallpaperWindow.hpp"
#include "../panel/TopPanel.hpp"
#include <gtkmm/application.h>
#include "../config/ThemeManager.hpp"
#include "../app_launcher/AppLauncher.hpp"
#include "../context_menu/DesktopContextMenu.hpp"
#include <memory> // Añadido para smart pointers

class CoreSystem {
public:
    CoreSystem(const std::string& theme_path = "config/theme.json");
    ~CoreSystem();

    void start(Glib::RefPtr<Gtk::Application> app);
    void stop();
    void reload_theme();
    void setup_context_menu();

private:
    // Cambiamos a unique_ptr para gestión automática de memoria
    std::unique_ptr<WallpaperWindow> wallpaper;
    std::unique_ptr<TopPanel> top_panel;
    std::unique_ptr<AppLauncher> app_launcher;
    Glib::RefPtr<Gtk::Application> app;
    std::unique_ptr<ThemeManager> theme;
    std::string theme_path;
    std::unique_ptr<DesktopContextMenu> context_menu;
};