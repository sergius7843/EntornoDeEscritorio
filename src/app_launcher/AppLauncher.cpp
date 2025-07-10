// AppLauncher.cpp
#include "AppLauncher.hpp"
#include <iostream>

AppLauncher::AppLauncher() 
    : main_box(Gtk::Orientation::VERTICAL),
      app1("Navegador"), app2("Editor de texto"), app3("Terminal") {
    
    set_title("App Launcher");
    get_style_context()->add_class("app-launcher");
    set_decorated(false);
    set_resizable(false);
    set_default_size(300, 200);

    // Conectar señales y guardar conexiones
    button_connections.push_back(
        app1.signal_clicked().connect([this]() { launch_dummy_app("Navegador"); })
    );
    button_connections.push_back(
        app2.signal_clicked().connect([this]() { launch_dummy_app("Editor"); })
    );
    button_connections.push_back(
        app3.signal_clicked().connect([this]() { launch_dummy_app("Terminal"); })
    );

    main_box.append(app1);
    main_box.append(app2);
    main_box.append(app3);
    set_child(main_box);
    hide();
}

AppLauncher::~AppLauncher() {
    // Desconectar todas las señales de botones
    for (auto& conn : button_connections) {
        conn.disconnect();
    }
    button_connections.clear();
    
    // Limpiar proveedor CSS
    if (current_provider) {
        auto context = get_style_context();
        context->remove_provider(current_provider);
        current_provider.reset();
    }
}

void AppLauncher::apply_theme(ThemeManager* theme) {
    auto context = get_style_context();
    
    // Limpiar proveedor anterior
    if (current_provider) {
        context->remove_provider(current_provider);
    }
    
    // Aplicar nuevo proveedor
    current_provider = theme->get_css_provider();
    context->add_provider(current_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void AppLauncher::toggle_visibility() {
    if (get_visible()) {
        hide();
    } else {
        show();
    }
}

void AppLauncher::launch_dummy_app(const Glib::ustring& name) {
    std::cout << "Simulando Lanzamiento de: " << name << std::endl;
}