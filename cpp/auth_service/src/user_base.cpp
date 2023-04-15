#include "user_base.h"

#include <db_session_manager.h>

#include <Poco/Data/Statement.h>

using namespace Poco::Data::Keywords;

void UserBase::initialize() {
    auto session = DatabaseSessionManager::get().getSession();

    session << "CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
               "`login` VARCHAR(30) NOT NULL UNIQUE, "
               "`password` VARCHAR(20) NOT NULL, `first_name` VARCHAR(30) NOT NULL, `last_name` VARCHAR(30) NOT NULL, "
               "`email` VARCHAR(50) NOT NULL UNIQUE)", Poco::Data::Keywords::now;
}

bool UserBase::authenticateUser(const std::string &login, const std::string &password) {
    std::vector<std::string> fetchedPasswords;

    auto session = DatabaseSessionManager::get().getSession();
    Poco::Data::Statement statement(session);

    statement << "SELECT password FROM User WHERE login=?", use(const_cast<std::string &>(login)), into(
            fetchedPasswords);

    statement.execute();

    if (fetchedPasswords.size() != 1) {
        return false;
    }

    return fetchedPasswords[0] == password;
}

UserBase::RegisteredUserInfo UserBase::registerUser(const User &user) {
    UserBase::RegisteredUserInfo result;
    auto &copy = const_cast<User &>(user);

    if (UserBase::findUserByLogin(user.login)) {
        result.result = UserBase::UserRegistrationResult::AlreadyExists;
        return result;
    }

    auto session = DatabaseSessionManager::get().getSession();
    Poco::Data::Statement insert(session);

    insert << "INSERT INTO User (login, password, first_name, last_name, email) VALUES(?, ?, ?, ?, ?)", use(
            copy.login), use(copy.password), use(copy.firstName), use(copy.lastName), use(copy.email);
    insert.execute();

    Poco::Data::Statement select(session);

    int userId;
    select << "SELECT LAST_INSERT_ID()", into(userId);
    select.execute();

    result.userId = userId;
    result.result = UserBase::UserRegistrationResult::Ok;

    return result;
}

std::optional<User> UserBase::findUserByLogin(const std::string &login) {
    User user;

    auto session = DatabaseSessionManager::get().getSession();
    Poco::Data::Statement statement(session);

    statement << "SELECT first_name, last_name, email FROM User WHERE User.login=?", use(
            const_cast<std::string &>(login)), into(user.firstName), into(user.lastName), into(user.email),
            range(0, 1);

    if (statement.execute() != 1) {
        return std::nullopt;
    }

    return user;
}

std::vector<User> UserBase::findUserByNameMasks(const std::string &firstNameMask, const std::string &lastNameMask) {
    std::vector<User> result;
    User fetchedUser;

    auto session = DatabaseSessionManager::get().getSession();
    Poco::Data::Statement statement(session);

    statement
            << "SELECT login, first_name, last_name, email FROM User WHERE first_name LIKE ? AND last_name LIKE ?", use(
            const_cast<std::string &>(firstNameMask)), use(const_cast<std::string &>(lastNameMask)),
            into(fetchedUser.login), into(fetchedUser.firstName), into(fetchedUser.lastName), into(fetchedUser.email),
            range(0, 1);

    while (!statement.done()) {
        statement.execute();
        result.push_back(std::move(fetchedUser));
    }

    return result;
}
