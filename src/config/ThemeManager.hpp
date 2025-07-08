// ThemeManager.hpp
#pragma once
#include <gtkmm.h>
#include <nlohmann/json.hpp>

class ThemeManager {
public:
    ThemeManager(const std::string& config_path);
    Glib::RefPtr<Gtk::CssProvider> get_css_provider() const;
    // recarga del tema
    void reload();

private:
    nlohmann::json theme_data;
    Glib::RefPtr<Gtk::CssProvider> css_provider;

    void load_theme();
    void generate_css();
    std::string config_path;

};
