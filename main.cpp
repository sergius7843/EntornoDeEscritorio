// main.cpp
#include "src/core/CoreSystem.hpp"
#include <gtkmm/application.h>
#include <iostream>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.mi.entorno");
    CoreSystem core("config/theme.json");;

    app->signal_activate().connect([&]() {
        core.start(app);
    });

    // Prueba de recarga después de 5 segundos
    Glib::signal_timeout().connect_seconds([&]() {
        std::cout << "Recargando tema..." << std::endl;
        core.reload_theme();
        return true; // Solo se ejecuta una vez
    }, 5);

    return app->run(argc, argv);
}
