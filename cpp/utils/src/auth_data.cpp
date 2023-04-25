#include "auth_data.h"

std::optional<AuthData> getAuthData(Poco::Net::HTTPServerRequest& request) {
    if (!request.hasCredentials()) {
        return std::nullopt;
    }

    std::string schema, credentials;
    request.getCredentials(schema, credentials);

    return AuthData{std::move(schema), std::move(credentials)};
}