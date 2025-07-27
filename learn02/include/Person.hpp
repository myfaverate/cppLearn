#ifndef PERSON_HPP
#define PERSON_HPP
#include <string>
#include <iostream>
class Person {
private:
    std::string name;
    int age;
public:
    Person(const std::string& name, int age);
    ~Person();

    std::string getName() const;
    int getAge() const;

    void setName(const std::string& name);
    void setAge(int age);

    friend std::ostream& operator<<(std::ostream& os, const Person& person);
};

#endif  // PERSON_HPP
