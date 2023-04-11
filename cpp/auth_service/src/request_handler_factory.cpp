#include "request_handler_factory.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace {
    class Handler : public Poco::Net::HTTPRequestHandler {
        void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override {
            response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
            response.setContentType("text/html");
            response.send() << request.getURI() << '\n';
        }
    };
}

Poco::Net::HTTPRequestHandler*
RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest&) {
    return new Handler();
}
