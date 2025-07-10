// src/utils/MemoryUtils.hpp
#pragma once

#include <memory>
#include <gtkmm.h>
#include <functional>
#include <vector>
#include <map>
#include <string>

/**
 * @brief Utilidades para gestión de memoria segura en el entorno de escritorio
 * 
 * Este archivo proporciona wrappers y utilidades para gestión automática de memoria
 * usando RAII (Resource Acquisition Is Initialization) y smart pointers.
 * 
 * Principios aplicados:
 * - RAII: Los recursos se adquieren en construcción y se liberan en destrucción
 * - Smart Pointers: Gestión automática de memoria heap
 * - Exception Safety: Garantiza liberación de recursos incluso con excepciones
 */

namespace MemoryUtils {

    /**
     * @brief Wrapper RAII para GtkCssProvider
     * 
     * Gestiona automáticamente el ciclo de vida de los proveedores CSS
     * evitando fugas de memoria y garantizando la limpieza correcta
     */
    class CssProviderManager {
    private:
        std::map<std::string, Glib::RefPtr<Gtk::CssProvider>> providers;
        
    public:
        CssProviderManager() = default;
        ~CssProviderManager() = default;
        
        // No permitir copia para evitar problemas de ownership
        CssProviderManager(const CssProviderManager&) = delete;
        CssProviderManager& operator=(const CssProviderManager&) = delete;
        
        // Permitir movimiento
        CssProviderManager(CssProviderManager&&) = default;
        CssProviderManager& operator=(CssProviderManager&&) = default;
        
        /**
         * @brief Crea o actualiza un proveedor CSS
         * @param name Nombre identificador del proveedor
         * @param css_data Datos CSS a aplicar
         * @return Referencia al proveedor creado/actualizado
         */
        Glib::RefPtr<Gtk::CssProvider> create_or_update_provider(const std::string& name, const std::string& css_data) {
            auto& provider = providers[name];
            if (!provider) {
                provider = Gtk::CssProvider::create();
            }
            
            try {
                provider->load_from_data(css_data);
            } catch (const Glib::Error& e) {
                // Log error but don't throw - permite degradación elegante
                std::cerr << "Error cargando CSS para '" << name << "': " << e.what() << std::endl;
            }
            
            return provider;
        }
        
        /**
         * @brief Obtiene un proveedor existente
         * @param name Nombre del proveedor
         * @return Proveedor si existe, nullptr si no
         */
        Glib::RefPtr<Gtk::CssProvider> get_provider(const std::string& name) {
            auto it = providers.find(name);
            return (it != providers.end()) ? it->second : nullptr;
        }
        
        /**
         * @brief Elimina un proveedor específico
         * @param name Nombre del proveedor a eliminar
         */
        void remove_provider(const std::string& name) {
            providers.erase(name);
        }
        
        /**
         * @brief Limpia todos los proveedores
         */
        void clear_all() {
            providers.clear();
        }
    };

    /**
     * @brief Wrapper RAII para gestión de eventos y callbacks
     * 
     * Gestiona automáticamente las conexiones de señales y eventos
     * garantizando que se desconecten correctamente al destruir el objeto
     */
    class EventConnectionManager {
    private:
        std::vector<sigc::connection> connections;
        
    public:
        EventConnectionManager() = default;
        
        ~EventConnectionManager() {
            disconnect_all();
        }
        
        // No permitir copia
        EventConnectionManager(const EventConnectionManager&) = delete;
        EventConnectionManager& operator=(const EventConnectionManager&) = delete;
        
        // Permitir movimiento
        EventConnectionManager(EventConnectionManager&&) = default;
        EventConnectionManager& operator=(EventConnectionManager&&) = default;
        
        /**
         * @brief Registra una conexión para gestión automática
         * @param connection Conexión a gestionar
         */
        void add_connection(sigc::connection connection) {
            connections.push_back(connection);
        }
        
        /**
         * @brief Desconecta todas las conexiones registradas
         */
        void disconnect_all() {
            for (auto& conn : connections) {
                if (conn.connected()) {
                    conn.disconnect();
                }
            }
            connections.clear();
        }
        
        /**
         * @brief Obtiene el número de conexiones activas
         * @return Número de conexiones activas
         */
        size_t active_connections() const {
            size_t count = 0;
            for (const auto& conn : connections) {
                if (conn.connected()) {
                    count++;
                }
            }
            return count;
        }
    };

    /**
     * @brief Wrapper RAII para gestión de recursos temporales
     * 
     * Útil para recursos que necesitan limpieza específica
     * como archivos temporales, conexiones de red, etc.
     */
    template<typename T>
    class ResourceManager {
    private:
        std::unique_ptr<T> resource;
        std::function<void(T*)> cleanup_func;
        
    public:
        ResourceManager(std::unique_ptr<T> res, std::function<void(T*)> cleanup)
            : resource(std::move(res)), cleanup_func(cleanup) {}
        
        ~ResourceManager() {
            if (resource && cleanup_func) {
                cleanup_func(resource.get());
            }
        }
        
        // No permitir copia
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        
        // Permitir movimiento
        ResourceManager(ResourceManager&&) = default;
        ResourceManager& operator=(ResourceManager&&) = default;
        
        T* get() const { return resource.get(); }
        T* operator->() const { return resource.get(); }
        T& operator*() const { return *resource; }
        
        /**
         * @brief Libera el recurso manualmente
         * @return Puntero al recurso liberado
         */
        T* release() {
            cleanup_func = nullptr;
            return resource.release();
        }
    };

    /**
     * @brief Factory para crear widgets con gestión de memoria automática
     * 
     * Simplifica la creación de widgets garantizando gestión correcta
     */
    class WidgetFactory {
    public:
        /**
         * @brief Crea un widget managed (GTK se encarga de la memoria)
         * @tparam WidgetType Tipo de widget a crear
         * @tparam Args Argumentos del constructor
         * @param args Argumentos para el constructor del widget
         * @return Puntero al widget creado (managed por GTK)
         */
        template<typename WidgetType, typename... Args>
        static WidgetType* create_managed(Args&&... args) {
            return Gtk::make_managed<WidgetType>(std::forward<Args>(args)...);
        }
        
        /**
         * @brief Crea un widget con smart pointer único
         * @tparam WidgetType Tipo de widget a crear
         * @tparam Args Argumentos del constructor
         * @param args Argumentos para el constructor del widget
         * @return unique_ptr al widget creado
         */
        template<typename WidgetType, typename... Args>
        static std::unique_ptr<WidgetType> create_unique(Args&&... args) {
            return std::make_unique<WidgetType>(std::forward<Args>(args)...);
        }
        
        /**
         * @brief Crea un widget con smart pointer compartido
         * @tparam WidgetType Tipo de widget a crear
         * @tparam Args Argumentos del constructor
         * @param args Argumentos para el constructor del widget
         * @return shared_ptr al widget creado
         */
        template<typename WidgetType, typename... Args>
        static std::shared_ptr<WidgetType> create_shared(Args&&... args) {
            return std::make_shared<WidgetType>(std::forward<Args>(args)...);
        }
    };

    /**
     * @brief Utilidad para debugging de memoria
     * 
     * Ayuda a rastrear el uso de memoria durante desarrollo
     */
    class MemoryDebugger {
    private:
        static std::map<std::string, size_t> allocation_counts;
        static std::map<std::string, size_t> deallocation_counts;
        
    public:
        static void log_allocation(const std::string& type) {
            allocation_counts[type]++;
        }
        
        static void log_deallocation(const std::string& type) {
            deallocation_counts[type]++;
        }
        
        static void print_summary() {
            std::cout << "\n=== RESUMEN DE MEMORIA ===\n";
            for (const auto& [type, count] : allocation_counts) {
                size_t dealloc_count = deallocation_counts[type];
                std::cout << type << ": " << count << " asignaciones, " 
                         << dealloc_count << " liberaciones";
                if (count != dealloc_count) {
                    std::cout << " [POSIBLE FUGA: " << (count - dealloc_count) << "]";
                }
                std::cout << "\n";
            }
            std::cout << "==========================\n\n";
        }
        
        static void clear_counters() {
            allocation_counts.clear();
            deallocation_counts.clear();
        }
    };

    // Macros para debugging opcional (solo en modo debug)
    #ifdef DEBUG_MEMORY
    #define MEMORY_LOG_ALLOC(type) MemoryUtils::MemoryDebugger::log_allocation(type)
    #define MEMORY_LOG_DEALLOC(type) MemoryUtils::MemoryDebugger::log_deallocation(type)
    #define MEMORY_PRINT_SUMMARY() MemoryUtils::MemoryDebugger::print_summary()
    #else
    #define MEMORY_LOG_ALLOC(type)
    #define MEMORY_LOG_DEALLOC(type)
    #define MEMORY_PRINT_SUMMARY()
    #endif

} // namespace MemoryUtils

// Inicialización de variables estáticas
std::map<std::string, size_t> MemoryUtils::MemoryDebugger::allocation_counts;
std::map<std::string, size_t> MemoryUtils::MemoryDebugger::deallocation_counts;