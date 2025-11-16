#include <iostream>
#include <memory>

#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysqlx/xdevapi.h>

struct User {
    uint64_t id;
    std::string username;
    std::string password;
    std::string email;
    int32_t age;
    std::string gender;
    explicit User()
        : id(0), username(""), password(""), email(""), age(0), gender("男") {
        std::cout << "默认构造 p: " << *this << "\n";
    }
    explicit User(const sql::ResultSet* resultSet)
        : id(resultSet->getUInt64("id")),
          username(resultSet->getString("username")),
          password(resultSet->getString("password")),
          email(resultSet->getString("email")),
          age(resultSet->getInt("age")),
          gender(resultSet->getString("gender")) {
        std::cout << "数据库1构造 p: " << *this << "\n";
    }
    explicit User(const mysqlx::Row& row)
        : id(row[0].get<uint64_t>()),
          username(row[1].get<std::string>()),
          password(row[2].get<std::string>()),
          email(row[3].get<std::string>()),
          age(row[4].get<int32_t>()),
          gender(row[5].get<std::string>()) {
        std::cout << "数据库2构造 p: " << *this << "\n";
    }
    void setId(const uint64_t id) { this->id = id; }
    void setUserName(const std::string username) { this->username = username; }
    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        os << std::format(
            "User(id={}, username={}, password={}, email={}, age={}, "
            "gender={})",
            user.id, user.username, user.password, user.email, user.age,
            user.gender);
        return os;
    }
};

int main(int, char**) {
    // sql 语句
    const std::string sql1 = R"(
        use test;
    )";
    const std::string sql2 = R"(
        select database();
    )";
    const std::string sql3 = R"(
        show tables;
    )";
    const std::string sql4 = R"(
        desc t_user;
    )";

    const std::string sql5 = R"(
        show create table t_user;
    )";

    const std::string sql6 = R"(
        select *
        from information_schema.columns
        where table_schema = 'test'
        and table_name = 't_user';
    )";

    // 增
    const std::string sql7 = R"(
        insert into t_user (username, password, email, age, gender)
        values ('tom', '123456', 'tom@qq.com', 20, '男'), ('tom1', '1234567', 'tom1@qq.com', 21, '男1');
    )";
    const std::string sql8 = R"(
        insert into t_user (username, password, email, age, gender)
        values (?, ?, ?, ?, ?);
    )";
    // 批量增加
    // 删
    // 改
    // 查
    const std::string sql11 = R"(
        select * from t_user;
    )";

    // 传统 api 模式
    sql::Driver* driver = get_driver_instance();
    const sql::SQLString hostName = "tcp://192.168.31.90";
    const sql::SQLString userName = "root";
    const sql::SQLString passWord = "root";
    sql::Connection* connection = driver->connect(hostName, userName, passWord);
    sql::Statement* statement = connection->createStatement();
    {
        const auto result =
            statement->execute(sql1);  // execute返回结果代表是否有
                                       // ResultSet，没有抛异常就证明执行成功
        std::cout << std::format("sql1执行是否成功: {}", result) << "\n";
    }
    sql::ResultSet* resultSet = statement->executeQuery(sql2);
    while (resultSet->next()) {
        const auto dbName = resultSet->getString(1);  // 获取第一列
        std::cout << "dbName: " << dbName << '\n';
    }
    resultSet = statement->executeQuery(sql3);
    while (resultSet->next()) {
        const auto tableName = resultSet->getString(1);  // 获取第一列
        std::cout << "tableName: " << tableName << '\n';
    }
    resultSet = statement->executeQuery(sql4);
    std::cout << "查看表结构\n";
    {
        // 查看有多少行，多少列，表头信息
        const auto columnCount = resultSet->getMetaData()->getColumnCount();
        std::cout << "columnCount: " << columnCount << "\n";
    }
    while (resultSet->next()) {
        const auto columnName = resultSet->getMetaData()->getColumnName(1);
        std::cout << "columnName: " << columnName << "\n";
        resultSet->getMetaData()->getTableName(1);
        const auto field = resultSet->getString(1);         // 获取第一列
        const auto type = resultSet->getString(2);          // 获取第一列
        const auto nullable = resultSet->getString(3);      // 获取第一列
        const auto key = resultSet->getString(4);           // 获取第一列
        const auto defaultValue = resultSet->getString(5);  // 获取第一列
        const auto tableName6 = resultSet->getString(6);    // 获取第一列
        std::cout << std::format(
            "field: {}, type: {}, nullable: {}, key: {}, defaultValue: {}, "
            "tableName6: {}\n",
            field.asStdString(), type.asStdString(), nullable.asStdString(),
            key.asStdString(), defaultValue.asStdString(),
            tableName6.asStdString());
    }
    // 插入
    // auto affectRows = statement->executeUpdate(sql7);
    // if(affectRows > 0){
    //     std::cout << "插入成功\n";
    // }
    // 安全插入 + 事务
    connection->setAutoCommit(false);
    try{
        // sql::PreparedStatement* prepareStatement = connection->prepareStatement(sql8);
        // const auto prepareStatement = std::make_unique<sql::PreparedStatement>(connection->prepareStatement(sql8)); // 没有构造函数的话不能这样调用
        const auto prepareStatement = std::unique_ptr<sql::PreparedStatement>(connection->prepareStatement(sql8));
        prepareStatement->setString(1, "lisa");
        prepareStatement->setString(2, "888888");
        prepareStatement->setString(3, "lisa@qq.com");
        prepareStatement->setInt(4, 22);
        prepareStatement->setString(5, "女");
        const auto rows = prepareStatement->executeUpdate();
        resultSet = statement->executeQuery("select last_insert_id();");
        if (resultSet->next()) {
            std::cout << "插入记录 ID = " << resultSet->getUInt64(1) << std::endl;
        }
        if (rows > 0) {
            std::cout << "插入成功, 是否有更多结果集: " << prepareStatement->getMoreResults() << "\n";
        }
        throw std::runtime_error("运行时异常1");
        connection->commit();
    } catch (sql::SQLException& e) {
        // 回滚事务
        connection->rollback();
        std::cout << "事务回滚, 原因: " << e.what() << std::endl;
    }

    // 查询
    resultSet = statement->executeQuery(sql11);
    {
        // 查看有多少行，多少列，表头信息
        const auto columnCount = resultSet->getMetaData()->getColumnCount();
        std::cout << "columnCount: " << columnCount << "\n";
        while (resultSet->next()) {
            User user(resultSet);
            std::cout << user << "\n";
        }
    }
    resultSet->close();
    statement->close();
    connection->close();
    delete resultSet;
    delete statement;
    delete connection;
    std::cout << "============================================================="
                 "====\n";
    // session 模式
    // 创建 Session 连接
    mysqlx::Session session(mysqlx::SessionOption::HOST, "192.168.31.90",
                            mysqlx::SessionOption::PORT, 33060,
                            mysqlx::SessionOption::USER, "root",
                            mysqlx::SessionOption::PWD, "root");

    mysqlx::SqlResult result = session.sql(sql1).execute();
    if (result.count() > 0) {
        std::cout << "执行成功\n";
    }
    result = session.sql(sql2).execute();
    // 使用 RowResult 迭代每一行
    mysqlx::Row row;
    while ((row = result.fetchOne())) {
        std::cout << row[0].get<std::string>() << std::endl;
    }
    result = session.sql(sql3).execute();
    while ((row = result.fetchOne())) {
        std::cout << row[0].get<std::string>() << std::endl;
    }
    std::cout << "查看表结构: \n";
    result = session.sql(sql4).execute();
    while ((row = result.fetchOne())) {
        const auto field = row[0].get<std::string>();     // 获取第一列
        const auto type = row[1].get<std::string>();      // 获取第一列
        const auto nullable = row[2].get<std::string>();  // 获取第一列
        const auto key = row[3].get<std::string>();       // 获取第一列
        const auto defaultValue =
            row[4].isNull() ? "" : row[4].get<std::string>();  // 获取第一列
        const auto tableName6 = row[5].get<std::string>();     // 获取第一列
        std::cout << std::format(
            "field: {}, type: {}, nullable: {}, key: {}, defaultValue: {}, "
            "tableName6: {}\n",
            field, type, nullable, key, defaultValue, tableName6);
    }
    // 插入
    // std::cout << "插入:\n";
    // result = session.sql(sql7).execute();
    // if(result.getAffectedItemsCount() > 0){
    //     std::cout << "插入成功\n";
    // }
    // 插入2
    session.startTransaction();
    try {
        result = session.sql(sql8)
            .bind("lisa", "888888", "lisa@qq.com", 22, "女")
            .execute();
        if(result.getAffectedItemsCount() > 0){
            std::cout << "插入成功, 主键id为: " << result.getAutoIncrementValue() << "\n";
        }
        throw std::runtime_error("运行时异常1");
        session.commit();
    } catch (const std::exception& e) {
        std::cerr << "session异常: " << e.what() << '\n';
    }
    
    std::cout << "查询:\n";
    result = session.sql(sql11).execute();
    while ((row = result.fetchOne())) {
        User user(row);
        std::cout << user << "\n";
    }
    // 事务
    session.close();
}