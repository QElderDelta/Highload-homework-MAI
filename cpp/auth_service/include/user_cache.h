#pragma once

#include "user.h"

#include <sw/redis++/redis++.h>

#include <chrono>
#include <optional>

class UserCache {
public:
    static UserCache& get();

    std::optional<std::string> getUserFromCache(const std::string& login);

    void addUserToCache(const std::string& login, const std::string& userInfo);

private:
    UserCache();

    constexpr static auto CacheEntryLifeTime = std::chrono::seconds(30);

    sw::redis::Redis cache_;
};