// CSSParser.hpp
#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <regex>

class CSSParser {
public:
    std::string parse(const std::string& css, const nlohmann::json& variables);
};