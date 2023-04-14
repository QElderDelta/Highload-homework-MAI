#pragma once

#include <string>

enum class AuthenticationResult {
    Authenticated,
    NotAuthenticated,
    BadCredentials,
    InternalError
};

struct AuthData {
    std::string schema;
    std::string credentials;
};

AuthenticationResult authenticateUser(const AuthData& authData);
