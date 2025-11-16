#include <learn001.hpp>
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void handleClick(){
    std::cout << "Button Clicked!\n";
    QMessageBox::information(nullptr, "Hello", "Clicked the Button");
}
bool initDatabase() {
    // ✅ 连接 SQLite 数据库（内存模式，也可以是文件路径）
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // db.setDatabaseName(":memory:"); // 或 "test.db"
    db.setDatabaseName("test.db"); // 或 "test.db"

    if (!db.open()) {
        qDebug() << "Failed to open database:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    // ✅ 创建表
    if (!query.exec("CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT);")) {
        qDebug() << "Create table failed:" << query.lastError().text();
        return false;
    }

    // ✅ 插入数据
    query.exec("INSERT INTO users (name) VALUES ('Alice');");
    query.exec("INSERT INTO users (name) VALUES ('Bob');");

    // ✅ 查询数据
    query.exec("SELECT id, name FROM users;");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        qDebug() << "User:" << id << name;
    }

    return true;
}
int main(int argc, char *argv[]){
    // QApplication app(__argc, __argv);

    // QPushButton button("Fetch Data from https://example.com");
    // button.resize(300, 60);
    // button.show();

    // QNetworkAccessManager* manager = new QNetworkAccessManager(&button);

    // QObject::connect(&button, &QPushButton::clicked, [&]() {
    //     QUrl url("https://www.baidu.com");
    //     QNetworkRequest request(url);

    //     QNetworkReply* reply = manager->get(request);

    //     QObject::connect(reply, &QNetworkReply::finished, [reply]() {
    //         if (reply->error() == QNetworkReply::NoError) {
    //             QByteArray data = reply->readAll();
    //             QMessageBox::information(nullptr, "Response", QString::fromUtf8(data.left(200))); // 截取前200字符
    //             std::cout << "Response Data: " << data.left(100000).toStdString() << "...\n";
    //         } else {
    //             QMessageBox::warning(nullptr, "Error", reply->errorString());
    //         }
    //         reply->deleteLater();
    //     });
    // });

    // return app.exec();

    // SQL
    // QApplication app(argc, argv);

    // QPushButton button("Init QtSQL Database");
    // button.resize(250, 60);
    // button.show();

    // QObject::connect(&button, &QPushButton::clicked, [&]() {
    //     if (initDatabase()) {
    //         QMessageBox::information(nullptr, "Success", "Database initialized and queried!");
    //     } else {
    //         QMessageBox::warning(nullptr, "Error", "Database operation failed!");
    //     }
    // });

    // return app.exec();

    // 谈随机一百个弹窗
    
}
