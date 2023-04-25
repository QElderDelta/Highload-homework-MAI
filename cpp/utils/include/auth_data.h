#pragma once

#include <Poco/Net/HTTPServerRequest.h>

#include <optional>
#include <string>

struct AuthData {
    std::string schema;
    std::string credentials;
};

std::optional<AuthData> getAuthData(Poco::Net::HTTPServerRequest& request);
