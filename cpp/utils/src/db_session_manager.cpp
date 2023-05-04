#include "db_session_manager.h"

#include <Poco/Data/MySQL/Connector.h>

#include <iostream>

namespace {
    std::string getConnectionString(const DatabaseConfig& config) {
        std::string result;

        result.append("host=").append(config.host);
        result.append(";user=").append(config.user);
        result.append(";password=").append(config.password);
        result.append(";db=").append(config.db_name);
        result.append(";port=").append(config.port);

        return result;
    }
}

DatabaseSessionManager::DatabaseSessionManager(const DatabaseConfig& config) {
    Poco::Data::MySQL::Connector::registerConnector();

    pool_ = std::make_unique<Poco::Data::SessionPool>(Poco::Data::MySQL::Connector::KEY,
                                                      getConnectionString(config));
}

DatabaseSessionManager& DatabaseSessionManager::get() {
    static DatabaseSessionManager manager(DatabaseConfig::get());

    return manager;
}

Poco::Data::Session DatabaseSessionManager::getSession() const {
    return pool_->get();
}