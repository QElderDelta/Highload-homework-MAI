#pragma once

#include "auth_data.h"

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

struct HttpRequestResult {
    Poco::Net::HTTPResponse response;
    std::string body;
};

HttpRequestResult sendHttpRequest(const std::string& host, int port, const std::string& uri,
                                  const std::string& method, const AuthData& credentials,
                                  const std::string& body);
