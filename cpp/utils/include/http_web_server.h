#pragma once

#include "i_service_initializer.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Util/ServerApplication.h>

#include <string>
#include <type_traits>
#include <vector>

template<typename ServiceInitializer, typename RequestHandlerFactory>
class HttpWebServer : public Poco::Util::ServerApplication {
protected:
    int main([[maybe_unused]] const std::vector<std::string>& args) override {
        static_assert(std::is_base_of_v<IServiceInitializer, ServiceInitializer>,
                      "ServiceInitializer must be a descendant of IServiceInitializer");
        static_assert(std::is_base_of_v<Poco::Net::HTTPRequestHandlerFactory, RequestHandlerFactory>,
                      "RequestHandlerFactory must be a descendant of Poco::Net::HTTPRequestHandlerFactory");

        ServiceInitializer{}.initialize();

        Poco::Net::HTTPServer server(new RequestHandlerFactory,
                                     std::stoi(std::getenv("SERVICE_PORT")));

        server.start();
        waitForTerminationRequest();
        server.stop();

        return Poco::Util::Application::EXIT_OK;
    }
};
