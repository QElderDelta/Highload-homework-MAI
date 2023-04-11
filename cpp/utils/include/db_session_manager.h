#include "db_config.h"

#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>

#include <memory>

class DatabaseSessionManager {
public:
    static DatabaseSessionManager& get();

    Poco::Data::Session getSession() const;
private:
    explicit DatabaseSessionManager(const DatabaseConfig& config);
private:
    std::unique_ptr<Poco::Data::SessionPool> pool_;
};