//AppLauncher.hpp
#pragma once
#include <gtkmm.h>
#include "../config/ThemeManager.hpp"

class AppLauncher : public Gtk::Window {
public:
    AppLauncher();
    void toggle_visibility(); // mostrar/ocultar

    void apply_theme(ThemeManager* theme);

private:
    Gtk::Box main_box;
    Gtk::Button app1, app2, app3;
    void launch_dummy_app(const Glib::ustring& name);

    Glib::RefPtr<Gtk::CssProvider> current_provider; // Para manejar CSS
};