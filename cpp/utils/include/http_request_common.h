#pragma once

#include "auth_data.h"

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

Poco::Net::HTTPResponse sendHttpRequest(const std::string& host, int port, const std::string& uri,
                                        const std::string& method, const AuthData& credentials);
