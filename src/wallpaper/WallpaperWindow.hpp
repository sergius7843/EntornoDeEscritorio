//WallpaperWindow.hpp
#pragma once
#include <gtkmm.h>
#include "../config/ThemeManager.hpp"
#include <gdkmm/event.h>

class WallpaperWindow : public Gtk::Window {
public:
    WallpaperWindow(const std::string& wallpaper_path); // Ruta configurable
    void apply_theme(ThemeManager* theme);

    // Nueva función para manejar eventos
    void setup_event_listeners();

    void refresh_desktop(); 

private:
    Gtk::Picture image;
    std::string current_wallpaper;
    Glib::RefPtr<Gtk::CssProvider> current_provider; // Nuevo: para manejar CSS

    // Para detección de clic derecho
    Glib::RefPtr<Gtk::GestureClick> right_click_gesture;
    void on_right_click_pressed(int n_press, double x, double y);

    Gtk::Widget* get_root_widget(); // Nuevo
};
