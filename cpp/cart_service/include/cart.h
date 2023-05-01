#pragma once

#include <string>
#include <unordered_map>

struct Cart {
    std::unordered_map<int, int> items;

    std::string toJson() const;
};
