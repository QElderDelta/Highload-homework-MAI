#include "cart_base.h"

#include <db_session_manager.h>

using namespace Poco::Data::Keywords;

void CartBase::initialize() {
    auto session = DatabaseSessionManager::get().getSession();

    session << "CREATE TABLE IF NOT EXISTS `Cart` (`id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, "
               "`user_id` INT NOT NULL, "
               "`product_id` INT NOT NULL, "
               "`quantity` INT NOT NULL, "
               "CONSTRAINT `fk_product_id` "
               "FOREIGN KEY (product_id) REFERENCES Product (id) "
               "ON DELETE CASCADE "
               "ON UPDATE RESTRICT);", now;
}

void CartBase::addItemToCart(int userId, int itemId, int quantity) {
    auto session = DatabaseSessionManager::get().getSession();

    int count = 0;
    Poco::Data::Statement select(session);
    select << "SELECT quantity FROM Cart WHERE user_id=? AND product_id=?", into(count), use(
            userId), use(itemId);
    select.execute();

    Poco::Data::Statement modify(session);
    if (count) {
        count += quantity;
        modify << "UPDATE Cart SET quantity=? WHERE user_id=? AND product_id=?", use(count), use(
                userId), use(itemId);
    } else {
        modify << "INSERT INTO Cart (user_id, product_id, quantity) VALUES(?, ?, ?)", use(
                userId), use(itemId), use(quantity);
    }

    modify.execute();
}

Cart CartBase::getCartForUser(int userId) {
    auto session = DatabaseSessionManager::get().getSession();

    Cart result;
    int itemId, quantity;

    Poco::Data::Statement select(session);

    select << "SELECT product_id, quantity FROM Cart WHERE user_id=?", into(itemId), into(
            quantity), use(userId), range(0, 1);

    while (!select.done()) {
        if (select.execute() == 1) {
            result.items[itemId] += quantity;
        }
    }

    return result;
}