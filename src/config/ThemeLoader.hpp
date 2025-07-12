// ThemeLoader.hpp
#pragma once
#include <string>
#include <functional>

class ThemeLoader {
public:
    using ThemeChangeCallback = std::function<void(const std::string& component_name)>;
    
    ThemeLoader(const std::string& theme_dir);
    void watch_for_changes();
    void register_component(const std::string& component_name, ThemeChangeCallback callback);
    
private:
    std::string theme_dir_;
    std::unordered_map<std::string, ThemeChangeCallback> callbacks_;
    
    // Implementación específica del sistema de archivos para monitoreo
    void setup_file_watcher();
};