// DesktopContextMenu.hpp
#pragma once
#include <gtkmm.h>
#include <vector>
#include <functional>
#include <memory>

struct MenuItem {
    std::string label;
    std::function<void()> action;
    std::string icon_name = "";
};

class DesktopContextMenu : public Gtk::Popover {
public:
    DesktopContextMenu();
    ~DesktopContextMenu() = default;
    
    void add_item(const MenuItem& item);
    void show_at_position(double x, double y);
    void set_parent_widget(Gtk::Widget* parent);

private:
    Gtk::Box menu_box;
    std::vector<MenuItem> items;
    
    void setup_menu();
};