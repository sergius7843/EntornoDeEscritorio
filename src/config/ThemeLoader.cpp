// ThemeLoader.cpp
#include "ThemeLoader.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ThemeLoader::ThemeLoader(const std::string& theme_dir)
    : theme_dir_(theme_dir) {
    if (!fs::exists(theme_dir_)) {
        fs::create_directories(theme_dir_);
    }
}

ThemeLoader::~ThemeLoader() {
    for (auto& [file, monitor] : monitors_) {
        if (monitor) {
            monitor->cancel();
        }
    }
    monitors_.clear();
}

void ThemeLoader::watch_for_changes() {
    std::string theme_config_path = theme_dir_ + "/theme.json";
    setup_file_monitor(theme_config_path, "global");
    
    // Monitorear el directorio de componentes
    setup_file_monitor(theme_dir_, "all");
}

void ThemeLoader::register_component(const std::string& component_name, ThemeChangeCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    callbacks_[component_name] = callback;
    
    std::string css_path = theme_dir_ + "/" + component_name + ".css";
    if (fs::exists(css_path)) {
        setup_file_monitor(css_path, component_name);
    }
}

void ThemeLoader::setup_file_monitor(const std::string& file_path, const std::string& component_name) {
    if (monitors_.find(file_path) != monitors_.end()) {
        return;
    }
    
    try {
        auto file = Gio::File::create_for_path(file_path);
        if (!file) {
            std::cerr << "Error: No se pudo crear objeto GFile para " << file_path << std::endl;
            return;
        }
        
        // GTK4 compatible - usar monitor_file sin flags
        Glib::RefPtr<Gio::FileMonitor> monitor;
        if (fs::is_directory(file_path)) {
            monitor = file->monitor_directory();
        } else {
            monitor = file->monitor_file();
        }
        
        if (!monitor) {
            std::cerr << "Error: No se pudo crear monitor para " << file_path << std::endl;
            return;
        }
        
        // Conectar señal de cambio
        monitor->signal_changed().connect(
            [this, component_name](const Glib::RefPtr<Gio::File>& file,
                                  const Glib::RefPtr<Gio::File>& other_file,
                                  Gio::FileMonitor::Event event_type) {
                this->on_file_changed(file, other_file, event_type, component_name);
            }
        );
        
        monitors_[file_path] = monitor;
        std::cout << "Monitoreando cambios en: " << file_path << std::endl;
    }
    catch (const Glib::Error& e) {
        std::cerr << "Error configurando monitor: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error general configurando monitor: " << e.what() << std::endl;
    }
}

void ThemeLoader::on_file_changed(const Glib::RefPtr<Gio::File>& file,
                                  const Glib::RefPtr<Gio::File>& other_file,
                                  Gio::FileMonitor::Event event_type,
                                  const std::string& component_name) {
    
    // Solo procesar eventos de cambio reales
    if (event_type != Gio::FileMonitor::Event::CHANGED &&
        event_type != Gio::FileMonitor::Event::CHANGES_DONE_HINT) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "Cambio detectado en: " << file->get_path() << std::endl;
    
    if (component_name == "global" || component_name == "all") {
        for (auto& [comp_name, callback] : callbacks_) {
            try {
                callback(comp_name);
            }
            catch (const std::exception& e) {
                std::cerr << "Error en callback para " << comp_name << ": " << e.what() << std::endl;
            }
        }
    }
    else {
        auto it = callbacks_.find(component_name);
        if (it != callbacks_.end()) {
            try {
                it->second(component_name);
            }
            catch (const std::exception& e) {
                std::cerr << "Error en callback para " << component_name << ": " << e.what() << std::endl;
            }
        }
    }
}