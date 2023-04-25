#include "authentication_request.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

bool sendAuthenticationRequest(const AuthData& credentials) {
    static std::string authServiceHost = std::getenv("AUTH_SERVICE_HOST");
    static int authServicePort = std::stoi(std::getenv("AUTH_SERVICE_PORT"));

    Poco::Net::HTTPClientSession session(authServiceHost, authServicePort);

    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, "/authenticate");
    request.setCredentials(credentials.schema, credentials.credentials);

    Poco::Net::HTTPResponse response;

    session.sendRequest(request);
    session.receiveResponse(response);

    return response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK;
}
