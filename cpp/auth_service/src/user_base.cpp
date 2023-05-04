#include "user_base.h"

#include <db_session_manager.h>

#include <Poco/Data/Statement.h>

using namespace Poco::Data::Keywords;

namespace {
    std::string getShardingComment(std::optional<int> shard = std::nullopt) {
        constexpr std::string_view ShardingCommentPrefix = "sharding:";

        std::string result = "-- ";

        result.append(ShardingCommentPrefix);

        if (shard) {
            result.append(std::to_string(*shard));
        } else {
            result.append("service");
        }

        return result;
    }
}

void UserBase::initialize() {
    auto session = DatabaseSessionManager::get().getSession();

    for (int shard = 0; shard < UserBase::ShardsCount; ++shard) {
        Poco::Data::Statement statement(session);

        statement
                << "CREATE TABLE IF NOT EXISTS `User` (`id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                   "`login` VARCHAR(30) NOT NULL UNIQUE, "
                   "`password` VARCHAR(20) NOT NULL, `first_name` VARCHAR(30) NOT NULL, `last_name` VARCHAR(30) NOT NULL, "
                   "`email` VARCHAR(50) NOT NULL UNIQUE) ";

        statement << " " << getShardingComment(shard);

        statement.execute();
    }

    Poco::Data::Statement statement(session);

    statement
            << "CREATE TABLE IF NOT EXISTS `UserList` (`id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY, "
               "`email` VARCHAR(50) NOT NULL UNIQUE) ";

    statement << " " << getShardingComment();

    statement.execute();
}

bool UserBase::authenticateUser(const std::string& login, const std::string& password) {
    std::vector<std::string> fetchedPasswords;

    auto session = DatabaseSessionManager::get().getSession();
    Poco::Data::Statement statement(session);

    statement << "SELECT password FROM User WHERE login=?", use(
            const_cast<std::string&>(login)), into(
            fetchedPasswords);
    statement << " " << getShardingComment(UserBase::getShard(login));

    statement.execute();

    if (fetchedPasswords.size() != 1) {
        return false;
    }

    return fetchedPasswords[0] == password;
}

UserBase::RegisteredUserInfo UserBase::registerUser(const User& user) {
    UserBase::RegisteredUserInfo result;
    auto& copy = const_cast<User&>(user);

    auto session = DatabaseSessionManager::get().getSession();

    if (UserBase::findUserByLogin(user.login)) {
        result.result = UserBase::UserRegistrationResult::AlreadyExists;
        return result;
    }

    {
        Poco::Data::Statement select(session);

        int count = 0;

        select << "SELECT COUNT(*) FROM UserList WHERE email=?", into(count), use(copy.email);
        select << " " << getShardingComment();

        select.execute();

        if (count != 0) {
            result.result = UserBase::UserRegistrationResult::AlreadyExists;
            return result;
        }
    }

    int userId;

    {
        Poco::Data::Statement insert(session);

        insert << "INSERT INTO UserList (email) VALUES(?)", use(copy.email);
        insert << " " << getShardingComment();
        insert.execute();

        Poco::Data::Statement select(session);

        select << "SELECT LAST_INSERT_ID()", into(userId);
        select << " " << getShardingComment();
        select.execute();
    }

    Poco::Data::Statement insert(session);

    insert
            << "INSERT INTO User (login, password, first_name, last_name, email) VALUES(?, ?, ?, ?, ?)", use(
            copy.login), use(copy.password), use(copy.firstName), use(copy.lastName), use(
            copy.email);
    insert << " " << getShardingComment(UserBase::getShard(copy.login));
    insert.execute();

    result.userId = userId;
    result.result = UserBase::UserRegistrationResult::Ok;

    return result;
}

std::optional<User> UserBase::findUserByLogin(const std::string& login) {
    User user;

    auto session = DatabaseSessionManager::get().getSession();
    Poco::Data::Statement statement(session);

    statement << "SELECT first_name, last_name, email FROM User WHERE User.login=?", use(
            const_cast<std::string&>(login)), into(user.firstName), into(user.lastName), into(
            user.email),
            range(0, 1);
    statement << " " << getShardingComment(UserBase::getShard(login));

    if (statement.execute() != 1) {
        return std::nullopt;
    }

    return user;
}

std::vector<User>
UserBase::findUserByNameMasks(const std::string& firstNameMask, const std::string& lastNameMask) {
    std::vector<User> result;
    User fetchedUser;

    auto session = DatabaseSessionManager::get().getSession();


    for (int shard = 0; shard < UserBase::ShardsCount; ++shard) {
        Poco::Data::Statement statement(session);

        statement
                << "SELECT login, first_name, last_name, email FROM User WHERE first_name LIKE ? AND last_name LIKE ?", use(
                const_cast<std::string&>(firstNameMask)), use(
                const_cast<std::string&>(lastNameMask)),
                into(fetchedUser.login), into(fetchedUser.firstName), into(
                fetchedUser.lastName), into(
                fetchedUser.email),
                range(0, 1);

        statement << " " << getShardingComment(shard);

        while (!statement.done()) {
            if (statement.execute() == 1) {
                result.push_back(std::move(fetchedUser));
            }
        }
    }

    return result;
}

int UserBase::getShard(std::string_view login) {
    return std::hash<std::string_view>{}(login) % UserBase::ShardsCount;
}
