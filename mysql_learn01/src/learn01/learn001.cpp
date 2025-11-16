#include "learn001.hpp"

namespace learn01 {
void hello() { std::cout << "Hello World\n"; }
void getPersonsFromMysql() {
    std::cout << "Hello getPersonsFromMysql\n";
    // sql::Driver *driver = get_driver_instance();
    // const sql::SQLString hostName = "tcp://localhost:3306";
    // const sql::SQLString userName = "root";
    // const sql::SQLString passWord = "root";
    // sql::Connection *connection = driver->connect(hostName, userName,
    // passWord); sql::Statement *statement = connection->createStatement();
    // sql::ResultSet *resultSet = statement->executeQuery("show databases;");
    // while(resultSet->next()){
    //     const auto dbName = resultSet->getString("Database");
    //     std::cout << "dbName: " << dbName << '\n';
    // }
    // resultSet->close();
    // statement->close();
    // connection->close();
}
}  // namespace learn01