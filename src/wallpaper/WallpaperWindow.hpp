// WallpaperWindow.hpp
#pragma once
#include <gtkmm.h>
#include "../config/ThemeManager.hpp"
#include <gdkmm/event.h>
#include <memory> // Para weak_ptr

class WallpaperWindow : public Gtk::Window {
public:
    WallpaperWindow(const std::string& wallpaper_path);
    ~WallpaperWindow(); // Destructor añadido
    
    void apply_theme(ThemeManager* theme);
    void setup_event_listeners();
    void refresh_desktop(); 

private:
    Gtk::Picture image;
    std::string current_wallpaper;
    Glib::RefPtr<Gtk::CssProvider> current_provider;

    // Gestos y señales
    Glib::RefPtr<Gtk::GestureClick> right_click_gesture;
    // sigc::connection refresh_connection; // Conexión para efecto de refresco
    
    void on_right_click_pressed(int n_press, double x, double y);
};