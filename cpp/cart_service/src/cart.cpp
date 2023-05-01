#include "cart.h"

#include <Poco/JSON/Object.h>

#include <sstream>

std::string Cart::toJson() const {
    Poco::JSON::Object::Ptr result = new Poco::JSON::Object();

    Poco::JSON::Array::Ptr itemsArray = new Poco::JSON::Array();

    for(const auto& item : items) {
        Poco::JSON::Object::Ptr cartItem = new Poco::JSON::Object();

        cartItem->set("item_id", item.first);
        cartItem->set("quantity", item.second);

        itemsArray->add(cartItem);
    }

    result->set("items", itemsArray);

    std::ostringstream os;
    Poco::JSON::Stringifier::stringify(result, os);
    return os.str();
}
