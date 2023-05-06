#include "user_validator.h"

#include <regex>
#include <unordered_map>

UserValidationResult UserValidator::validate(const User& user) {
    using ValidationFunc = std::function<bool(const User&)>;

    static const std::unordered_map<UserValidationResult, ValidationFunc> resultToFunc = {
            {UserValidationResult::InvalidLogin,     UserValidator::validateLogin},
            {UserValidationResult::InvalidPassword,  UserValidator::validatePassword},
            {UserValidationResult::InvalidEmail,     UserValidator::validateEmail},
            {UserValidationResult::InvalidFirstName, UserValidator::validateFirstName},
            {UserValidationResult::InvalidLastName,  UserValidator::validateLastName},
    };

    for (const auto& [result, func]: resultToFunc) {
        if (!func(user)) {
            return result;
        }
    }

    return UserValidationResult::Ok;
}

bool UserValidator::validateLogin(const User& user) {
    return !user.login.empty() && user.login.size() <= 30;
}

bool UserValidator::validatePassword(const User& user) {
    return !user.password.empty() && user.password.size() <= 20;
}

bool UserValidator::validateEmail(const User& user) {
    if (user.email.empty() || user.email.size() > 50) {
        return false;
    }

    static std::regex emailRegex(R""(^[\w-\.]+@([\w-]+\.)+[\w-]{2,15}$)"");

    return std::regex_match(user.email, emailRegex);
}

bool UserValidator::validateFirstName(const User& user) {
    return !user.firstName.empty() && user.firstName.size() <= 30;
}

bool UserValidator::validateLastName(const User& user) {
    return !user.lastName.empty() && user.firstName.size() <= 30;
}
