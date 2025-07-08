// TopPanel.hpp
#pragma once
#include "../config/ThemeManager.hpp"
#include <gtkmm.h>

class AppLauncher; // Declaración adelantada

class TopPanel : public Gtk::Window {
public:
    TopPanel();
    ~TopPanel();
    
    void set_app_launcher(AppLauncher* launcher); // Nuevo
    void apply_theme(ThemeManager* theme);

private:
    Gtk::Box box;
    Gtk::Label clock;
    sigc::connection timer_connection;
    bool update_time();
    
    Gtk::Button menu_button;
    AppLauncher* app_launcher = nullptr; // Inicializado como nullptr

    // Para manejar CSS
    Glib::RefPtr<Gtk::CssProvider> current_provider;
};