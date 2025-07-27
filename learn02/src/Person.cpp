#include "Person.hpp"
Person::Person(const std::string& name, int age) : name(name), age(age) {
    std::cout << "Person created: " << name << ", Age: " << age << std::endl;
}
Person::~Person() {
    std::cout << "Person destroyed: " << "name: " << name << std::endl;
}
std::string Person::getName() const {
    return name;
}
int Person::getAge() const {
    return age;
}
void Person::setName(const std::string& name) {
    this->name = name;
}
void Person::setAge(int age) {
    this->age = age;
}
std::ostream& operator<<(std::ostream& os, const Person& person) {
    os << "Name: " << person.name << ", Age: " << person.age;
    return os;
}