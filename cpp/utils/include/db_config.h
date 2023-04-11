#pragma once

#include <string_view>

struct DatabaseConfig {
    static DatabaseConfig& get();

    std::string_view db_name;
    std::string_view host;
    std::string_view password;
    std::string_view port;
    std::string_view user;
private:
    explicit DatabaseConfig();
};