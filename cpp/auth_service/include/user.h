#pragma once

#include <string>

struct User {
    std::string login;
    std::string firstName;
    std::string lastName;
    std::string password;
    std::string email;

    std::string toJson() const;

    std::string toJsonWithPassword() const;

    static User fromJson(const std::string& json);
};
