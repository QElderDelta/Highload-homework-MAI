#include "default_request_handler.h"

#include <Poco/Net/HTTPServerResponse.h>

void DefaultHandler::handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse& response) {
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
    response.send();
}
