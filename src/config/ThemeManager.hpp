#pragma once
#include <gtkmm.h>
#include <nlohmann/json.hpp>
#include <map>
#include <string>
#include <unordered_map>
#include <memory>
#include "ThemeLoader.hpp"             // Para carga dinámica de temas
#include "../utils/CSSParser.hpp"      // Procesamiento de variables CSS

class ThemeManager {
public:
    ThemeManager(const std::string& theme_dir);
    
    void reload();
    void reload_component(const std::string& component_name);
    
    Glib::RefPtr<Gtk::CssProvider> get_component_provider(const std::string& component_name) const;
    const nlohmann::json& get_global_vars() const;

private:
    void load_global_theme();
    void load_component_styles();
    void process_component_css(const std::string& component_name, const std::string& css_path);
    
    std::string theme_dir_;
    nlohmann::json global_vars_;
    std::unordered_map<std::string, Glib::RefPtr<Gtk::CssProvider>> component_providers_;
    std::unique_ptr<CSSParser> css_parser_;

    std::unique_ptr<ThemeLoader> theme_loader_; 
};
