#pragma once

#include <string>

struct Product {
    std::string name;
    std::string category;
    int price;
    int id;

    std::string toJson() const;
};
