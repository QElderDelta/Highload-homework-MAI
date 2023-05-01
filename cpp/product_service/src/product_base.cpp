#include "product_base.h"

#include <db_session_manager.h>

using namespace Poco::Data::Keywords;

void ProductBase::initialize() {
    auto session = DatabaseSessionManager::get().getSession();

    session
            << "CREATE TABLE IF NOT EXISTS `Product` (`id` INT NOT NULL PRIMARY KEY AUTO_INCREMENT, "
               "`name` VARCHAR(50) NOT NULL, `category` VARCHAR(50) NOT NULL, `price` INT NOT NULL)", now;
}

int ProductBase::addProduct(const Product& product) {
    auto& copy = const_cast<Product&>(product);

    auto session = DatabaseSessionManager::get().getSession();
    Poco::Data::Statement insert(session);

    insert << "INSERT INTO Product (name, category, price) VALUES (?, ?, ?)", use(copy.name), use(
            copy.category), use(copy.price);
    insert.execute();

    int productId;
    Poco::Data::Statement select(session);

    select << "SELECT LAST_INSERT_ID()", into(productId);
    select.execute();

    return productId;
}

std::vector<Product> ProductBase::getAllProducts() {
    Product fetchedProduct;
    std::vector <Product> result;

    auto session = DatabaseSessionManager::get().getSession();
    Poco::Data::Statement statement(session);

    statement << "SELECT id, name, category, price FROM Product", into(fetchedProduct.id), into(
            fetchedProduct.name), into(
            fetchedProduct.category), into(fetchedProduct.price), range(0, 1);

    while (!statement.done()) {
        statement.execute();

        result.push_back(std::move(fetchedProduct));
    }

    return result;
}
