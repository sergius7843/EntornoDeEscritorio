// DesktopContextMenu.cpp
#include "DesktopContextMenu.hpp"
#include <iostream>


DesktopContextMenu::DesktopContextMenu()
    : menu_box(Gtk::Orientation::VERTICAL) {
    set_child(menu_box);
    menu_box.set_margin(10);
    menu_box.set_spacing(5);
    set_autohide(true);
    set_has_arrow(false);
}

void DesktopContextMenu::add_item(const MenuItem& item) {
    items.push_back(item);
}

void DesktopContextMenu::setup_menu() { 
    // Limpiar widgets existentes usando el método correcto de GTK4
    auto child = menu_box.get_first_child();
    while (child) {
        auto next = child->get_next_sibling();
        menu_box.remove(*child);
        child = next;
    }

    // Crear nuevos widgets usando make_managed para gestión automática
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
        
        // Capturar 'this' de forma segura
        button->signal_clicked().connect([this, item]() {
            item.action();
            popdown();
        });
        
        menu_box.append(*button);
    }
}

void DesktopContextMenu::show_at_position(double x, double y) {
    setup_menu();
    Gdk::Rectangle rect(static_cast<int>(x), static_cast<int>(y), 1, 1);
    set_pointing_to(rect);
    popup();
}

void DesktopContextMenu::set_parent_widget(Gtk::Widget* parent) {
    if (parent) {
        set_parent(*parent);
    }
}

void DesktopContextMenu::apply_theme(ThemeManager* theme) {
    auto context = get_style_context();
    
    // Limpiar proveedor anterior si existe
    if (current_provider) {
        context->remove_provider(current_provider);
    }
    
    // Obtener proveedor específico para desktop-context-menu
    current_provider = theme->get_component_provider("desktop-context-menu");
    if (current_provider) {
        context->add_provider(current_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}