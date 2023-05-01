#include "http_request_common.h"

#include <Poco/Net/HTTPClientSession.h>

Poco::Net::HTTPResponse sendHttpRequest(const std::string& host, int port, const std::string& uri,
                                        const std::string& method, const AuthData& credentials) {
    Poco::Net::HTTPClientSession session(host, port);

    Poco::Net::HTTPRequest request(method, uri);
    request.setCredentials(credentials.schema, credentials.credentials);

    Poco::Net::HTTPResponse response;

    session.sendRequest(request);
    session.receiveResponse(response);

    return response;
}
