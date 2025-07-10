// AppLauncher.hpp
#pragma once
#include <gtkmm.h>
#include "../config/ThemeManager.hpp"
#include <sigc++/connection.h> // Para conexiones de señales

class AppLauncher : public Gtk::Window {
public:
    AppLauncher();
    ~AppLauncher(); // Destructor añadido
    
    void toggle_visibility();
    void apply_theme(ThemeManager* theme);

private:
    Gtk::Box main_box;
    Gtk::Button app1, app2, app3;
    
    // Conexiones de señales para manejo seguro
    std::vector<sigc::connection> button_connections;
    
    void launch_dummy_app(const Glib::ustring& name);
    Glib::RefPtr<Gtk::CssProvider> current_provider;
};