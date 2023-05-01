#pragma once

#include "cart.h"

class CartBase {
public:
    static void initialize();

    static void addItemToCart(int userId, int itemId, int quantity);

    static Cart getCartForUser(int userId);
};
