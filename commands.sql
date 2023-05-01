CREATE TABLE IF NOT EXISTS `User` (
    `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `login` VARCHAR(30) NOT NULL UNIQUE,
    `password` VARCHAR(20) NOT NULL,
    `first_name` VARCHAR(30) NOT NULL,
    `last_name` VARCHAR(30) NOT NULL,
    `email` VARCHAR(50) NOT NULL UNIQUE
);
CREATE TABLE IF NOT EXISTS `Product` (
    `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `name` VARCHAR(50) NOT NULL,
    `category` VARCHAR(50) NOT NULL,
    `price` INT NOT NULL
);
CREATE TABLE IF NOT EXISTS `Cart` (
    `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    `user_id` INT NOT NULL,
    `product_id` INT NOT NULL,
    `quantity` INT NOT NULL,
    CONSTRAINT `fk_product_id` FOREIGN KEY (product_id) REFERENCES Product (id) ON DELETE CASCADE ON UPDATE RESTRICT,
    CONSTRAINT `fk_user_id` FOREIGN KEY (user_id) REFERENCES Product (id) ON DELETE CASCADE ON UPDATE RESTRICT
);
INSERT INTO User (login, password, first_name, last_name, email)
VALUES('rofl', '1337322', 'Rofl', 'Kofl', 'xyz@ya.ru');
INSERT INTO User (login, password, first_name, last_name, email)
VALUES(
        'heisenberg',
        '1337322',
        'Walter',
        'White',
        'xyz@mail.ru'
    );
INSERT INTO User (login, password, first_name, last_name, email)
VALUES(
        'asac',
        '1337322',
        'Hank',
        'Schrader',
        'xyz@gmail.com'
    );
INSERT INTO Product (name, category, price)
VALUES ('Vacuum cleaner', 'Electronics', 1233);
INSERT INTO Product (name, category, price)
VALUES ('TV', 'Electronics', 1337);
INSERT INTO Product (name, category, price)
VALUES ('M&Ms', 'Food', 288);
INSERT INTO Cart (user_id, product_id, quantity)
VALUES(1, 1, 10);
INSERT INTO Cart (user_id, product_id, quantity)
VALUES(1, 2, 2);
INSERT INTO Cart (user_id, product_id, quantity)
VALUES(2, 3, 10);
SELECT *
FROM User;
SELECT *
FROM Product;
SELECT *
FROM Cart;