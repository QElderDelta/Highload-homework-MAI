#pragma once

#include "auth_data.h"

#include <string>

enum class AuthenticationResult {
    Authenticated,
    NotAuthenticated,
    BadCredentials,
    InternalError
};

AuthenticationResult authenticateUser(const AuthData& authData);
