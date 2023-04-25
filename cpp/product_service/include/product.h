#pragma once

#include <string>

struct Product {
    std::string name;
    std::string category;
    int price;

    std::string toJson() const;
};
