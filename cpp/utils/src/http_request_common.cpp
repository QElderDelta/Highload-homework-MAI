#include "http_request_common.h"

#include <Poco/Net/HTTPClientSession.h>

HttpRequestResult sendHttpRequest(const std::string& host, int port, const std::string& uri,
                                  const std::string& method, const AuthData& credentials,
                                  const std::string& body) {
    Poco::Net::HTTPClientSession session(host, port);

    Poco::Net::HTTPRequest request(method, uri);
    request.setCredentials(credentials.schema, credentials.credentials);

    Poco::Net::HTTPResponse response;

    auto& requestStream = session.sendRequest(request);

    requestStream << body;

    auto& responseStream = session.receiveResponse(response);

    HttpRequestResult result;

    std::string temp;
    while (std::getline(responseStream, temp)) {
        result.body.append(temp).append("\n");
    }

    result.response = response;

    return result;
}
