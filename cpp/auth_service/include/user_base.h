#pragma once

#include "user.h"

#include <optional>
#include <string>
#include <vector>

class UserBase {
public:
    enum class UserRegistrationResult {
        Ok,
        AlreadyExists
    };

    struct RegisteredUserInfo {
        UserRegistrationResult result;
        std::optional<int> userId;
    };

    static void initialize();

    static bool authenticateUser(const std::string& login, const std::string& password);

    static RegisteredUserInfo registerUser(const User& user);

    static std::optional<User> findUserByLogin(const std::string& login);

    static std::vector<User>
    findUserByNameMasks(const std::string& firstNameMask, const std::string& lastNameMask);

private:

    static int getShard(std::string_view login);

    static constexpr int ShardsCount = 2;
};
