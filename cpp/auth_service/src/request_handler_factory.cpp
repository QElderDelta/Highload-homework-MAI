#include "authentication_common.h"
#include "default_request_handler.h"
#include "healthcheck_handler.h"
#include "kafka_config.h"
#include "request_handler_factory.h"
#include "user_base.h"
#include "user_cache.h"
#include "user_validator.h"

#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <iostream>

namespace {
    void handleAuthenticationResult(AuthenticationResult result,
                                    Poco::Net::HTTPServerResponse& response) {
        switch (result) {
            case AuthenticationResult::NotAuthenticated:
                [[fallthrough]];
            case AuthenticationResult::BadCredentials:
                response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
                break;
            case AuthenticationResult::InternalError:
                response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                break;
            default:
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        }
        response.send();
    }

    class AuthenticationHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            if (auto authData = getAuthData(request)) {
                handleAuthenticationResult(authenticateUser(*authData), response);
            } else {
                handleAuthenticationResult(AuthenticationResult::BadCredentials, response);
            }
        }
    };

    class RegistrationHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            Poco::Net::HTMLForm body(request, request.stream());

            const auto hasAllData = [&body]() {
                constexpr std::array requiredFields = {"login", "first_name", "last_name",
                                                       "password", "email"};

                return std::all_of(requiredFields.begin(), requiredFields.end(),
                                   [&body](const char* field) { return body.has(field); });
            }();

            if (!hasAllData) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            User user{body.get("login"), body.get("first_name"), body.get("last_name"),
                      body.get("password"),
                      body.get("email")};

            if (UserValidator::validate(user) != UserValidationResult::Ok) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            UserBase::RegisteredUserInfo info;

            try {
                static cppkafka::Producer producer(KafkaConfig::get());

                const auto msg = user.toJsonWithPassword();

                while (true) {
                    try {
                        producer.produce(
                                cppkafka::MessageBuilder(KafkaConfig::getTopic()).payload(msg));
                        break;
                    } catch (...) {

                    }
                }

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            } catch (...) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
            }

            response.send();
        }
    };

    class SearchByLoginHandler : public Poco::Net::HTTPRequestHandler {
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            if (auto authData = getAuthData(request)) {
                if (auto authResult = authenticateUser(*authData); authResult !=
                                                                   AuthenticationResult::Authenticated) {
                    handleAuthenticationResult(authResult, response);
                    return;
                }
            } else {
                handleAuthenticationResult(AuthenticationResult::BadCredentials, response);
                return;
            }

            Poco::Net::HTMLForm body(request, request.stream());

            if (!body.has("login")) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            const std::string userLogin = body.get("login");

            try {
                std::string userJson;

                if (auto cacheValue = UserCache::get().getUserFromCache(userLogin); cacheValue) {
                    userJson = *cacheValue;
                } else {
                    auto foundUser = UserBase::findUserByLogin(userLogin);

                    if (foundUser) {
                        userJson = foundUser->toJson();

                        UserCache::get().addUserToCache(userLogin, userJson);
                    }
                }


                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");

                response.send() << userJson;
            } catch (...) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                response.send();
            }
        }
    };

    class SearchByNameHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            if (auto authData = getAuthData(request)) {
                if (auto authResult = authenticateUser(*authData); authResult !=
                                                                   AuthenticationResult::Authenticated) {
                    handleAuthenticationResult(authResult, response);
                    return;
                }
            } else {
                handleAuthenticationResult(AuthenticationResult::BadCredentials, response);
                return;
            }

            Poco::Net::HTMLForm body(request, request.stream());

            if (!body.has("first_name") || !body.has("last_name")) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            try {
                auto foundUsers = UserBase::findUserByNameMasks(body.get("first_name"),
                                                                body.get("last_name"));

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");

                auto& stream = response.send();

                if (!foundUsers.empty()) {
                    stream << "{";

                    for (size_t i = 0; i < foundUsers.size(); ++i) {
                        stream << foundUsers[i].toJson();

                        if (i != foundUsers.size() - 1) {
                            stream << ",";
                        }
                    }

                    stream << "}";
                }
            } catch (...) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                response.send();
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
        return new AuthenticationHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST && hasSubstr(uri, "/register")) {
        return new RegistrationHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, "/search_by_login")) {
        return new SearchByLoginHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, "/search_by_name")) {
        return new SearchByNameHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, HealthcheckHandler::HealthcheckUri)) {
        return new HealthcheckHandler();
    }

    return new DefaultHandler();
}
