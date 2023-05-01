#include "auth_data.h"
#include "authentication_request.h"
#include "default_request_handler.h"
#include "healthcheck_handler.h"
#include "product.h"
#include "product_base.h"
#include "product_validator.h"
#include "request_handler_factory.h"

#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace {
    class CreationHandler : public Poco::Net::HTTPRequestHandler {
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

            if (!form.has("name") || !form.has("category") || !form.has("price")) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            Product product{form.get("name"), form.get("category"), std::stoi(form.get("price")),
                            -1};

            if (ProductValidator::validate(product) !=
                ProductValidator::ProductValidationResult::Ok) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                response.send();
                return;
            }

            try {
                int productId = ProductBase::addProduct(product);

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");

                response.send() << "{\"product_id\": " << productId << "}";
            } catch (...) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
                response.send();
            }
        }
    };

    class GetAllHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response) override {
            if (auto credentials = getAuthData(request); !credentials ||
                                                         !sendAuthenticationRequest(*credentials)) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
                response.send();
                return;
            }

            try {
                auto products = ProductBase::getAllProducts();

                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");

                auto& stream = response.send();

                if (!products.empty()) {
                    stream << "{";

                    for (size_t i = 0; i < products.size(); ++i) {
                        stream << products[i].toJson();

                        if (i != products.size() - 1) {
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

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST &&
        hasSubstr(uri, "/add_product")) {
        return new CreationHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, "/get_all_products")) {
        return new GetAllHandler();
    }

    if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET &&
        hasSubstr(uri, HealthcheckHandler::HealthcheckUri)) {
        return new HealthcheckHandler();
    }

    return new DefaultHandler();
}
