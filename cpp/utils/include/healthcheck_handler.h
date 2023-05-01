#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

class HealthcheckHandler : public Poco::Net::HTTPRequestHandler {
public:
    static constexpr auto* HealthcheckUri = "/healthcheck";

    void handleRequest(Poco::Net::HTTPServerRequest&, Poco::Net::HTTPServerResponse& response) override;
};
