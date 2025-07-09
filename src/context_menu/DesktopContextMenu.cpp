// DesktopContextMenu.cpp
#include "DesktopContextMenu.hpp"
#include <iostream>

DesktopContextMenu::DesktopContextMenu()
    : menu_box(Gtk::Orientation::VERTICAL) {
    set_child(menu_box);
    menu_box.set_margin(10);
    menu_box.set_spacing(5);
    
    // Configurar el popover
    set_autohide(true);
    set_has_arrow(false);
}

void DesktopContextMenu::add_item(const MenuItem& item) {
    items.push_back(item);
}

void DesktopContextMenu::setup_menu() {
    // Limpiar menú existente
    auto child = menu_box.get_first_child();
    while (child) {
        auto next = child->get_next_sibling();
        menu_box.remove(*child);
        child = next;
    }

    // Crear nuevos botones
    for (const auto& item : items) {
        auto button = Gtk::make_managed<Gtk::Button>();
        
        if (!item.icon_name.empty()) {
            auto icon = Gio::ThemedIcon::create(item.icon_name);
            auto image = Gtk::make_managed<Gtk::Image>(icon);
            button->set_child(*image);
            button->set_tooltip_text(item.label);
        } else {
            button->set_label(item.label);
        }
        
        button->signal_clicked().connect([this, item]() {
            item.action();
            this->popdown(); // Cerrar menú después de selección
        });
        
        menu_box.append(*button);
    }
}

void DesktopContextMenu::show_at_position(double x, double y) {
    setup_menu(); // Reconstruir menú
    
    // Establecer la posición del popover
    Gdk::Rectangle rect;
    rect.set_x(static_cast<int>(x));
    rect.set_y(static_cast<int>(y));
    rect.set_width(1);
    rect.set_height(1);
    
    set_pointing_to(rect);
    popup();
}