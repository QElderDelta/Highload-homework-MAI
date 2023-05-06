#include "user.h"

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

#include <sstream>

std::string User::toJson() const {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object();

    if (!login.empty()) {
        result->set("login", login);
    }

    if (!firstName.empty()) {
        result->set("firstName", firstName);
    }

    if (!lastName.empty()) {
        result->set("lastName", lastName);
    }

    if (!email.empty()) {
        result->set("email", email);
    }

    std::ostringstream os;
    Poco::JSON::Stringifier::stringify(result, os);
    return os.str();
}

std::string User::toJsonWithPassword() const {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object();

    if (!login.empty()) {
        result->set("login", login);
    }

    if (!firstName.empty()) {
        result->set("firstName", firstName);
    }

    if (!lastName.empty()) {
        result->set("lastName", lastName);
    }

    if (!email.empty()) {
        result->set("email", email);
    }

    if (!password.empty()) {
        result->set("password", password);
    }

    std::ostringstream os;
    Poco::JSON::Stringifier::stringify(result, os);
    return os.str();
}

User User::fromJson(const std::string& json) {
    User result;

    Poco::JSON::Parser parser;
    auto parsed = parser.parse(json);
    Poco::JSON::Object::Ptr parsedJson = parsed.extract<Poco::JSON::Object::Ptr>();

    result.login = parsedJson->getValue<std::string>("login");
    result.firstName = parsedJson->getValue<std::string>("firstName");
    result.lastName = parsedJson->getValue<std::string>("lastName");
    result.email = parsedJson->getValue<std::string>("email");
    result.password = parsedJson->getValue<std::string>("password");

    return result;
}
