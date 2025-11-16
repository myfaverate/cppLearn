#include "learn001.hpp"

struct Person{
    std::string name;
    int32_t age;
    std::string sex;
    double weight;
    Person() : name(""), age(0), sex("男"), weight(0.0){
        std::cout << "Person Construct1...\n";
    }
    Person(const std::string name, const int age, const std::string sex, const double weight) : name(name), age(age), sex(sex), weight(weight){
        std::cout << "Person Construct2...\n";
    }
    ~Person(){
        std::cout << "Person Deconstruct...\n";
    }
    void setName(const std::string name){
        this->name = name;
    }
    void setAge(const int age){
        this->age = age;
    }
     void setSex(const std::string sex){
        this->sex = sex;
    }
     void setWeight(const double weight){
        this->weight = weight;
    }
    friend std::ostream& operator<<(std::ostream& os, const Person& person){
        os << std::format("Person(name={}, age={}, sex={}, weight={})", person.name, person.age, person.sex, person.weight);
        return os;
    }
};

namespace learn01 {
    void hello(){
        std::cout << "Hello World\n";
    }
    void parseXmlToPerson(){
        pugi::xml_document document;
        const pugi::xml_parse_result result = document.load_file("../hello.xml");
        if(result.status != pugi::status_ok){
            std::cerr << result.description() << "\n";
            return;
        }
        pugi::xml_node personNode = document.child("Person");
        const auto id = personNode.attribute("id").value();
        std::cout << "id: " << id << "\n";
        const auto name = personNode.child_value("name");
        const auto age = personNode.child_value("age");
        const auto sex = personNode.child_value("sex");
        const auto weight = personNode.child_value("weight");
        Person p(name, std::stoi(age), sex, std::stod(weight));
        std::cout << p << "\n";

    }
    void parseXmlToPerson1(){
        std::ifstream xmlFile("../hello.xml");
        if(!xmlFile.is_open()){
            std::cerr << "文件打开失败\n";
            return;
        }
        std::string line;
        while(std::getline(xmlFile, line)){
            std::cout << line << "\n";
        }
        std::cout << "=========================\n";
        xmlFile.clear();
        xmlFile.seekg(std::streampos(0LL));
        std::string xmlContent{
            std::istreambuf_iterator<char>(xmlFile),
            std::istreambuf_iterator<char>()
        };
        std::cout << xmlContent << "\n";
        // 开始解析
        rapidxml::xml_document<> doc;
        // doc.parse<0>(&xmlContent[0]); // 取首char数组元素指针
        doc.parse<0>(xmlContent.data()); // C++ 17
        const auto personNode = doc.first_node();
        const auto nameNode = personNode->first_node("name");
        const auto ageNode = personNode->first_node("age");
        const auto sexNode = personNode->first_node("sex");
        const auto weightNode = personNode->first_node("weight");
        std::cout << std::format(
            "根节点name: {} -> value: {}, 子节点name: {} -> value: {}, "
            "子节点name: {} -> value: {}, 子节点name: {} -> value: {}, "
            "子节点name: {} -> value: {}",
            personNode->name(), personNode->value(), nameNode->name(),
            nameNode->value(), ageNode->name(), ageNode->value(),
            sexNode->name(), sexNode->value(), weightNode->name(),
            weightNode->value());
        Person p(nameNode->value(), std::stoi(ageNode->value()), sexNode->value(), std::stod(weightNode->value()));
        std::cout << p << "\n";
        xmlFile.close();
    }
    void parseXmlToPerson2(){
        tinyxml2::XMLDocument doc;
        const auto result = doc.LoadFile("../hello.xml");
        if(result != tinyxml2::XML_SUCCESS){
            std::cout << "加载xml文件失败\n";
            return;
        }
        const auto personNode = doc.FirstChildElement("Person");
        for(auto item = personNode->FirstChildElement("name"); item; item = item->NextSiblingElement()){
            std::cout << item->GetText() << "\n";
        }
        const auto nameNode = personNode->FirstChildElement("name");
        const auto ageNode = nameNode->NextSiblingElement();
        const auto sexNode = ageNode->NextSiblingElement();
        const auto weightNode = sexNode->NextSiblingElement();
        try{
            std::cout << nameNode->GetText() << '\n';
            std::cout << ageNode->GetText() << '\n';
            std::cout << sexNode->GetText() << '\n';
            std::cout << weightNode->GetText() << '\n';
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
        Person p(nameNode->GetText(), std::stoi(ageNode->GetText()), sexNode->GetText(), std::stod(weightNode->GetText()));
        std::cout << p << "\n";
    }
    void getPersonsFromMysql(){
        sql::Driver *driver = get_driver_instance();
        const sql::SQLString hostName = "tcp://localhost:3306";
        const sql::SQLString userName = "root";
        const sql::SQLString passWord = "root";
        sql::Connection *connection = driver->connect(hostName, userName, passWord);
        sql::Statement *statement = connection->createStatement();
        sql::ResultSet *resultSet = statement->executeQuery("show databases;");
        while(resultSet->next()){
            const auto dbName = resultSet->getString("Database");
            std::cout << "dbName: " << dbName << '\n';
        }
        resultSet->close();
        statement->close();
        connection->close();
    }
}
// https://dev.mysql.com/downloads/connector/cpp/
// https://dev.mysql.com/doc/dev/connector-cpp/latest/usage.html