#include "product_validator.h"

ProductValidator::ProductValidationResult ProductValidator::validate(const Product& product) {
    if (!validateName(product.name)) {
        return ProductValidationResult::InvalidName;
    }

    if (!validateCategory(product.category)) {
        return ProductValidationResult::InvalidCategory;
    }

    if (!validatePrice(product.price)) {
        return ProductValidationResult::InvalidPrice;
    }

    return ProductValidationResult::Ok;
}

bool ProductValidator::validateName(const std::string& name) {
    return !name.empty() && name.size() <= 50;
}

bool ProductValidator::validateCategory(const std::string& category) {
    return !category.empty() && category.size() <= 50;
}

bool ProductValidator::validatePrice(int price) {
    return price > 0;
}
