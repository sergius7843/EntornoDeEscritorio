// EventManager.cpp
#include "EventManager.hpp"

EventManager& EventManager::get_instance() {
    static EventManager instance;
    return instance;
}

void EventManager::register_event(const std::string& name, Callback callback) {
    events[name] = callback;
}

void EventManager::trigger_event(const std::string& name) {
    if (events.find(name) != events.end()) {
        events[name]();
    }
}