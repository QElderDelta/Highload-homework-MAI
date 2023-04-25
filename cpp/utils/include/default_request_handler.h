#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

class DefaultHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse& response) override;
};
