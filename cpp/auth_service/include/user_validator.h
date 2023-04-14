#pragma once

#include <user.h>

#include <string>

enum class UserValidationResult {
    Ok,
    InvalidLogin,
    InvalidPassword,
    InvalidEmail,
    InvalidFirstName,
    InvalidLastName
};

class UserValidator {
public:
    static UserValidationResult validate(const User& user);

private:
    static bool validateLogin(const User& user);

    static bool validatePassword(const User& user);

    static bool validateEmail(const User& user);

    static bool validateFirstName(const User& user);

    static bool validateLastName(const User& user);
};
