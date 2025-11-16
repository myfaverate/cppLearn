#include "learn001.hpp"

class Person {
private:
    std::string name;
public:
    // 构造函数
     Person(const std::string& name) : name(name) {
        std::cout << "Person constructed: " << name << std::endl;
    }
    // 析构函数
    ~Person() {
        std::cout << "Person destructed: " << name << std::endl;
    }
    // 拷贝构造函数
    Person(const Person& other) : name(other.name) {
        std::cout << "Person copy-constructed: " << name << std::endl;
    }
    // 移动构造函数
    Person(Person&& other) noexcept : name(std::move(other.name)) {
        std::cout << "Person move-constructed: " << name << std::endl;
    }
    // 赋值运算符
    Person& operator=(const Person& other) {
        if (this != &other) {
            name = other.name;
            std::cout << "Person copy-assigned: " << name << std::endl;
        }
        return *this;
    }
    // 移动赋值运算符
    Person& operator=(Person&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            std::cout << "Person move-assigned: " << name << std::endl;
        }
        return *this;
    }
    const std::string& getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }
    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        std::format("");
        os << "Person(Name: " << person.name << ")";
        return os;
    }
};


void consume(Person& p){
    std::cout << "consume(Person&): " << p << " (lvalue overload)\n";   
}

void consume(Person&& p) {
    std::cout << "consume(Person&&): " << p << " (rvalue overload)\n";
}

// template<typename T>
// void wrapper_forward(T&& arg) { // arg is a universal reference (万能引用) 虽然类型是右值类型,但是arg是一个左值
//     std::cout << "wrapper_forward(T&&): called\n";
//     consume(std::forward<T>(arg));
//     // consume(std::forward<T&>(arg)); // error
//     // consume(std::forward<T&&>(arg)); // error
//     // consume(arg);
// }

template<typename T>
void wrapper_forward(T&& arg) { 
    std::cout << "wrapper_forward(T&): called\n";
    std::cout << "T is Person?   " << std::is_same_v<T, Person> << "\n";
    std::cout << "T is Person&?  " << std::is_same_v<T, Person&> << "\n";
    std::cout << "T is Person&&?   " << std::is_same_v<T, Person&&> << "\n";

    std::cout << "arg type is Person?   " << std::is_same_v<decltype(arg), Person> << "\n";
    std::cout << "arg type is Person&?  " << std::is_same_v<decltype(arg), Person&> << "\n";
    std::cout << "arg type is Person&&?   " << std::is_same_v<decltype(arg), Person&&> << "\n";
    std::cout << "param type: " << (std::is_lvalue_reference_v<decltype(arg)> ? "lvalue ref" : "rvalue ref") << "\n";
    consume(std::forward<T>(arg));
    // consume(std::forward<T&>(arg)); // error
    // consume(std::forward<T&&>(arg)); // error
    // consume(arg);
}


template <typename T>
T&& forward(typename std::remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

void learn001() {
    std::cout << "Hello\n";
}

/*
    std::cout << std::boolalpha;
    std::cout << "T is int&?  " << std::is_same_v<T, int&> << "\n";
    std::cout << "T is int?   " << std::is_same_v<T, int> << "\n";
    std::cout << "param type: " << (std::is_lvalue_reference_v<decltype(x)> ? "lvalue ref" : "rvalue ref") << "\n";
    std::cout << "----\n";
 */

/*
std::forward的行为​​
std::forward<T>的逻辑：
如果 T是 ​​非引用类型​​（如 Person），std::forward会返回 T&&（右值引用）。
如果 T是 ​​左值引用​​（如 Person&），std::forward会返回 T&（左值引用）。
*/