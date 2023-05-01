#include "product.h"

#include <Poco/JSON/Object.h>

#include <sstream>

std::string Product::toJson() const {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object();

    result->set("id", id);
    result->set("name", name);
    result->set("category", category);
    result->set("price", price);

    std::ostringstream os;
    Poco::JSON::Stringifier::stringify(result, os);
    return os.str();
}
