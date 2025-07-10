// ThemeManager.cpp
#include "ThemeManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

ThemeManager::ThemeManager(const std::string& config_path)
    : config_path(config_path), css_provider(Gtk::CssProvider::create()) {
    load_theme();
    generate_css();
}

void ThemeManager::load_theme() {
    std::ifstream file(config_path);
    if (!file) {
        std::cerr << "No se pudo abrir " << config_path << std::endl;
        return;
    }

    try {
        file >> theme_data;
    } catch (const std::exception& e) {
        std::cerr << "Error leyendo el JSON: " << e.what() << std::endl;
    }
}

void ThemeManager::generate_css() {
    std::ostringstream css;

    std::string bg = theme_data.value("primary_color", "#1e1e1e");
    std::string fg = theme_data.value("text_color", "#ffffff");
    std::string accent = theme_data.value("accent_color", "#007acc");
    std::string hover = theme_data.value("button_hover", "#444444");
    std::string font_family = theme_data.value("font_family", "Sans");
    int font_size = theme_data.value("font_size", 10);


    css << "* {\n"
        << "  background-color: " << bg << ";\n"
        << "  color: " << fg << ";\n"
        << "  font-family: " << font_family << ";\n"
        << "  font-size: " << font_size << "px;\n"
        << "}\n"
        << "button:hover {\n"
        << "  background-color: " << hover << ";\n"
        << "}\n"
        << "window.top-panel {\n"
        << "  background-color: " << accent << ";\n"
        << "}";

    try {
        css_provider->load_from_data(css.str());
    } catch (const Glib::Error& e) {
        std::cerr << "Error aplicando CSS: " << e.what() << std::endl;
    }

    // Si existe custom_css en el JSON, lo agregamos crudo al final
    if (theme_data.contains("custom_css")) {
        css << theme_data["custom_css"].get<std::string>() << "\n";
    }
    try {
        // Crear NUEVO proveedor CSS en lugar de reutilizar
        css_provider = Gtk::CssProvider::create();
        css_provider->load_from_data(css.str());
    } catch (const Glib::Error& e) {
        std::cerr << "Error aplicando CSS: " << e.what() << std::endl;
    }

}

void ThemeManager::reload() {
    load_theme();
    generate_css();     // Regenera completamente el css
}


Glib::RefPtr<Gtk::CssProvider> ThemeManager::get_css_provider() const {
    return css_provider;
}