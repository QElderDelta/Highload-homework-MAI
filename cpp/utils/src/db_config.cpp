#include "db_config.h"

#include <cstdlib>

DatabaseConfig::DatabaseConfig() {
    db_name = std::getenv("MYSQL_DATABASE");
    host = std::getenv("DB_HOST");
    password = std::getenv("MYSQL_PASSWORD");
    port = std::getenv("DB_PORT");
    user = std::getenv("MYSQL_USER");
}

DatabaseConfig &DatabaseConfig::get() {
    static DatabaseConfig config;

    return config;
}