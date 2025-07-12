// CCSParser.cpp
#include "CSSParser.hpp"

std::string CSSParser::parse(const std::string& css, const nlohmann::json& variables) {
    std::string result = css;
    
    // Expresión regular para encontrar variables CSS: var(--variable-name)
    std::regex var_regex(R"(var\((--[a-zA-Z0-9_-]+)\))");
    std::smatch matches;
    
    std::string::const_iterator search_start = css.cbegin();
    while (std::regex_search(search_start, css.cend(), matches, var_regex)) {
        std::string full_match = matches[0];
        std::string var_name = matches[1];
        
        // Buscar el valor en las variables globales
        std::string replacement;
        if (variables.contains(var_name.substr(2))) { // Eliminar '--' del nombre
            replacement = variables[var_name.substr(2)].get<std::string>();
        } else {
            // Valor por defecto si no se encuentra
            replacement = "inherit";
        }
        
        // Reemplazar en el resultado
        size_t pos = result.find(full_match);
        if (pos != std::string::npos) {
            result.replace(pos, full_match.length(), replacement);
        }
        
        search_start = matches.suffix().first;
    }
    
    return result;
}