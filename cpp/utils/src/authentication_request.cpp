#include "authentication_request.h"
#include "http_request_common.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>

bool sendAuthenticationRequest(const AuthData& credentials) {
    static std::string authServiceHost = std::getenv("AUTH_SERVICE_HOST");
    static int authServicePort = std::stoi(std::getenv("AUTH_SERVICE_PORT"));

    return sendHttpRequest(authServiceHost, authServicePort, "/authenticate",
                           Poco::Net::HTTPRequest::HTTP_GET, credentials,
                           "").response.getStatus() ==
           Poco::Net::HTTPResponse::HTTP_OK;
}
