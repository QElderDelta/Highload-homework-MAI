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
