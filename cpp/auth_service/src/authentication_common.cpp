#include "authentication_common.h"
#include "user_base.h"

#include <Poco/Base64Decoder.h>

#include <optional>
#include <sstream>

namespace {
    using ExtractedLoginAndPassword = std::optional<std::pair<std::string, std::string>>;

    ExtractedLoginAndPassword extractLoginAndPassword(const std::string& encodedStr) {
        std::istringstream is(encodedStr);
        Poco::Base64Decoder decoder(is);

        std::string decodedString(std::istreambuf_iterator<char>(decoder), std::istreambuf_iterator<char>{});

        auto idx = decodedString.find(':');
        if (idx == std::string::npos) {
            return std::nullopt;
        }

        return ExtractedLoginAndPassword{std::in_place, decodedString.substr(0, idx),
                                         decodedString.substr(idx + 1)};
    }
}

AuthenticationResult authenticateUser(const AuthData& authData) {
    if (authData.schema != "Basic") {
        return AuthenticationResult::BadCredentials;
    }

    auto extracted = extractLoginAndPassword(authData.credentials);

    if (!extracted) {
        return AuthenticationResult::BadCredentials;
    }

    auto [login, password] = *extracted;

    try {
        if (UserBase::authenticateUser(login, password)) {
            return AuthenticationResult::Authenticated;
        } else {
            return AuthenticationResult::NotAuthenticated;
        }
    } catch (...) {
        return AuthenticationResult::InternalError;
    }
}
