#include "user_cache.h"

namespace {
    std::string getConnectionString() {
        static const std::string ConnectionString = std::string("tcp://").append(
                getenv("CACHE_HOST")).append(":").append(std::getenv("CACHE_PORT"));
        return ConnectionString;
    }
}

UserCache& UserCache::get() {
    static UserCache cache;
    return cache;
}

std::optional<std::string> UserCache::getUserFromCache(const std::string& login) {
    return cache_.get(login);
}

void UserCache::addUserToCache(const std::string& login, const std::string& userInfo) {
    cache_.set(login, userInfo, UserCache::CacheEntryLifeTime);
}

UserCache::UserCache() : cache_(sw::redis::Redis(getConnectionString())) {}

