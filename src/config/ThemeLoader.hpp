#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <glibmm.h>
#include <giomm.h>  // AÑADIDO: Necesario para Gio::FileMonitor
#include <mutex>

class ThemeLoader {
public:
    using ThemeChangeCallback = std::function<void(const std::string& component_name)>;
    
    ThemeLoader(const std::string& theme_dir);
    ~ThemeLoader();
    
    void watch_for_changes();
    void register_component(const std::string& component_name, ThemeChangeCallback callback);
    
    ThemeLoader(const ThemeLoader&) = delete;
    ThemeLoader& operator=(const ThemeLoader&) = delete;

private:
    std::string theme_dir_;
    std::unordered_map<std::string, ThemeChangeCallback> callbacks_;
    std::unordered_map<std::string, Glib::RefPtr<Gio::FileMonitor>> monitors_;
    std::mutex mutex_;
    
    void setup_file_monitor(const std::string& file_path, const std::string& component_name);
    void on_file_changed(const Glib::RefPtr<Gio::File>& file,
                         const Glib::RefPtr<Gio::File>& other_file,
                         Gio::FileMonitor::Event event_type,
                         const std::string& component_name);
};