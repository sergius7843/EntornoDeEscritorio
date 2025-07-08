//WallpaperWindow.hpp
#pragma once
#include <gtkmm.h>
#include "../config/ThemeManager.hpp"

class WallpaperWindow : public Gtk::Window {
public:
    WallpaperWindow(const std::string& wallpaper_path); // Ruta configurable
    void apply_theme(ThemeManager* theme);


private:
    Gtk::Picture image;
    std::string current_wallpaper;
    Glib::RefPtr<Gtk::CssProvider> current_provider; // Nuevo: para manejar CSS
};
