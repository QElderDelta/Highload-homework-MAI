#include "healthcheck_handler.h"

#include <Poco/Net/HTTPServerResponse.h>

void HealthcheckHandler::handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse& response) {
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
    response.send();
}
