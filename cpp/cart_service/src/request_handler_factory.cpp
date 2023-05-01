#include "auth_data.h"
#include "authentication_request.h"
#include "cart_base.h"
#include "default_request_handler.h"
#include "healthcheck_handler.h"
#include "request_handler_factory.h"

#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace {
    class AddItemHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            if (auto credentials = getAuthData(request); !credentials ||
                                                         !sendAuthenticationRequest(*credentials)) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
                response.send();
                return;
            }

            Poco::Net::HTMLForm form(request, request.stream());

            if (!form.has("user_id") || !form.has("product_id") || !form.has("quantity")) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            int userId = std::stoi(form.get("user_id"));
            int productId = std::stoi(form.get("product_id"));
            int quantity = std::stoi(form.get("quantity"));

            if (quantity <= 0) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            try {
                CartBase::addItemToCart(userId, productId, quantity);

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.send();
            } catch (...) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                response.send();
            }
        }
    };

    class GetCartHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            if (auto credentials = getAuthData(request); !credentials ||
                                                         !sendAuthenticationRequest(*credentials)) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
                response.send();
                return;
            }

            Poco::Net::HTMLForm body(request, request.stream());

            if (!body.has("user_id")) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            int userId = std::stoi(body.get("user_id"));

            try {
                auto cart = CartBase::getCartForUser(userId);

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");

                response.send() << cart.toJson();
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

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST && hasSubstr(uri, "/add_item")) {
        return new AddItemHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET && hasSubstr(uri, "/get_cart")) {
        return new GetCartHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, HealthcheckHandler::HealthcheckUri)) {
        return new HealthcheckHandler();
    }

    return new DefaultHandler();
}
