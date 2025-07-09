// EventManager.hpp
#pragma once
#include <functional>
#include <map>
#include <string>

class EventManager {
public:
    using Callback = std::function<void()>;
    
    static EventManager& get_instance();
    
    void register_event(const std::string& name, Callback callback);
    void trigger_event(const std::string& name);
    
private:
    EventManager() = default;
    std::map<std::string, Callback> events;
};