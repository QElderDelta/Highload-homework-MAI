#pragma once

#include "product.h"

#include <vector>

class ProductBase {
public:
    static void initialize();

    static int addProduct(const Product &product);

    static std::vector<Product> getAllProducts();
};
