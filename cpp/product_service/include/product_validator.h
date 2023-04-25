#pragma once

#include "product.h"

class ProductValidator {
public:
    enum class ProductValidationResult {
        InvalidName,
        InvalidCategory,
        InvalidPrice,
        Ok
    };

    static ProductValidationResult validate(const Product& product);

private:
    static bool validateName(const std::string& name);

    static bool validateCategory(const std::string& category);

    static bool validatePrice(int price);
};