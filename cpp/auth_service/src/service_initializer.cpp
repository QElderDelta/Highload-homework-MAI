#include "../include/service_initializer.h"

#include <db_session_manager.h>

void ServiceInitializer::initialize() {
    createUserTable();
}

void ServiceInitializer::createUserTable() {
    auto session = DatabaseSessionManager::get().getSession();

    session << "CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT, `login` VARCHAR(30) NOT NULL, "
               "`password` VARCHAR(20) NOT NULL, `first_name` VARCHAR(30) NOT NULL, `last_name` VARCHAR(30) NOT NULL), "
               "`email` VARCHAR(50) NOT NULL", Poco::Data::Keywords::now;
}
