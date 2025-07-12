// ThemeManager.cpp
#include "ThemeManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace fs = std::filesystem;

ThemeManager::ThemeManager(const std::string& theme_dir)
    : theme_dir_(theme_dir), css_parser_(std::make_unique<CSSParser>()) {
    reload();
}

void ThemeManager::reload() {
    load_global_theme();
    load_component_styles();
}

void ThemeManager::reload_component(const std::string& component_name) {
    // Recargar solo un componente específico
    std::string config_path = theme_dir_ + "/theme.json";
    std::ifstream file(config_path);
    
    if (!file) {
        std::cerr << "No se pudo abrir " << config_path << std::endl;
        return;
    }
    
    try {
        nlohmann::json theme_config;
        file >> theme_config;
        
        if (theme_config["components"].contains(component_name)) {
            std::string css_path = theme_dir_ + "/" + 
                                  theme_config["components"][component_name].get<std::string>();
            process_component_css(component_name, css_path);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error recargando componente: " << e.what() << std::endl;
    }
}

void ThemeManager::load_global_theme() {
    std::string config_path = theme_dir_ + "/theme.json";
    std::ifstream file(config_path);
    
    if (!file) {
        std::cerr << "No se pudo abrir " << config_path << std::endl;
        return;
    }
    
    try {
        nlohmann::json theme_config;
        file >> theme_config;
        global_vars_ = theme_config["global"];
    } catch (const std::exception& e) {
        std::cerr << "Error leyendo el JSON global: " << e.what() << std::endl;
    }
}

void ThemeManager::load_component_styles() {
    std::string config_path = theme_dir_ + "/theme.json";
    std::ifstream file(config_path);
    
    if (!file) {
        std::cerr << "No se pudo abrir " << config_path << std::endl;
        return;
    }
    
    try {
        nlohmann::json theme_config;
        file >> theme_config;
        
        if (!theme_config.contains("components")) {
            std::cerr << "No se encontró la sección 'components' en theme.json" << std::endl;
            return;
        }
        
        auto components = theme_config["components"];
        for (auto& [component_name, css_file] : components.items()) {
            std::string css_path = theme_dir_ + "/" + css_file.get<std::string>();
            process_component_css(component_name, css_path);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error cargando componentes: " << e.what() << std::endl;
    }
}

void ThemeManager::process_component_css(const std::string& component_name, const std::string& css_path) {
    if (!fs::exists(css_path)) {
        std::cerr << "Archivo CSS no encontrado: " << css_path << std::endl;
        return;
    }
    
    std::ifstream css_file(css_path);
    std::stringstream buffer;
    buffer << css_file.rdbuf();
    std::string css_content = buffer.str();
    
    // Procesar variables CSS
    std::string processed_css = css_parser_->parse(css_content, global_vars_);
    
    auto provider = Gtk::CssProvider::create();
    try {
        provider->load_from_data(processed_css);
        component_providers_[component_name] = provider;
    } catch (const Glib::Error& e) {
        std::cerr << "Error aplicando CSS para " << component_name << ": " << e.what() << std::endl;
    }
}

Glib::RefPtr<Gtk::CssProvider> ThemeManager::get_component_provider(const std::string& component_name) const {
    auto it = component_providers_.find(component_name);
    if (it != component_providers_.end()) {
        return it->second;
    }
    return nullptr;
}

const nlohmann::json& ThemeManager::get_global_vars() const {
    return global_vars_;
}