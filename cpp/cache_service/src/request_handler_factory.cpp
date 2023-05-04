#include "authentication_request.h"
#include "default_request_handler.h"
#include "healthcheck_handler.h"
#include "http_request_common.h"
#include "request_handler_factory.h"
#include "user_cache.h"

#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace {
    std::string getRequestBody(Poco::Net::HTTPServerRequest& request) {
        std::string result;
        std::string temp;

        auto& requestStream = request.stream();

        while (std::getline(requestStream, temp)) {
            result.append(temp).append("\n");
        }

        return result;
    }

    HttpRequestResult forwardHttpRequestToAuthService(Poco::Net::HTTPServerRequest& request) {
        static std::string authServiceHost = std::getenv("AUTH_SERVICE_HOST");
        static int authServicePort = std::atoi(std::getenv("AUTH_SERVICE_PORT"));

        if (auto credentials = getAuthData(request); credentials) {
            return sendHttpRequest(authServiceHost, authServicePort,
                                   request.getURI(),
                                   request.getMethod(),
                                   *credentials, getRequestBody(request));

        }

        HttpRequestResult result;
        result.response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
        return result;
    }

    void forwardHttpRequestToAuthService(Poco::Net::HTTPServerRequest& request,
                                         Poco::Net::HTTPServerResponse& response) {
        auto forwardedRequest = forwardHttpRequestToAuthService(request);

        response.setStatus(forwardedRequest.response.getStatus());

        if (auto& stream = response.send(); !forwardedRequest.body.empty()) {
            stream << forwardedRequest.body;
        }
    }

    class ForwardingHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            forwardHttpRequestToAuthService(request, response);
        }
    };

    class SearchByLoginHandler : public Poco::Net::HTTPRequestHandler {
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            if (auto credentials = getAuthData(request); !credentials ||
                                                         !sendAuthenticationRequest(*credentials)) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
                response.send();
                return;
            }

            Poco::Net::HTMLForm form(request, request.stream());

            if (!form.has("login")) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            const std::string login = form.get("login");

            if (auto userInfo = UserCache::get().getUserFromCache(login); userInfo) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.send() << *userInfo;
                return;
            }

            auto forwardedRequest = forwardHttpRequestToAuthService(request);

            if (forwardedRequest.response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK &&
                !forwardedRequest.body.empty()) {
                UserCache::get().addUserToCache(login, forwardedRequest.body);
            }

            response.setStatus(forwardedRequest.response.getStatus());

            if (auto& stream = response.send(); !forwardedRequest.body.empty()) {
                stream << forwardedRequest.body;
            }
        }
    };
}

Poco::Net::HTTPRequestHandler*
RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request) {
    static auto hasSubstr = [](const std::string& text, std::string_view pattern) {
        return text.find(pattern) != std::string::npos;
    };

    const auto& uri = request.getURI();

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET && hasSubstr(uri, "/auth")) {
        return new ForwardingHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST && hasSubstr(uri, "/register")) {
        return new ForwardingHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, "/search_by_login")) {
        return new SearchByLoginHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, "/search_by_name")) {
        return new ForwardingHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, HealthcheckHandler::HealthcheckUri)) {
        return new HealthcheckHandler();
    }

    return new DefaultHandler();
}
