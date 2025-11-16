#include <array>
#include <chrono>
#include <cstring>
#include <format>
#include <forward_list>
#include <iostream>
#include <list>
#include <sstream>
#include <thread>
#include <vector>

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

class Person {
   private:
    std::string name;

   public:
    // 构造函数
    Person(const std::string& name) : name(name) {
        std::cout << "Person constructed: " << name << std::endl;
    }
    // 析构函数
    ~Person() { std::cout << "Person destructed: " << name << std::endl; }
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
    void setName(const std::string& name) { this->name = name; }
    friend std::ostream& operator<<(std::ostream& os, const Person& person) {
        os << "Person(Name=" << person.name << ")";
        return os;
    }
};

// TODO
#include <variant>
template <size_t n, typename... T>
constexpr std::variant<T...> _tuple_index(const std::tuple<T...>& tpl, size_t i) {
    if constexpr (n >= sizeof...(T)) throw std::out_of_range("越界.");
    if (i == n)
        return std::variant<T...>{std::in_place_index<n>, std::get<n>(tpl)};
    return _tuple_index<(n < sizeof...(T) - 1 ? n + 1 : 0)>(tpl, i);
}
template <typename... T>
constexpr std::variant<T...> tuple_index(const std::tuple<T...>& tpl,
                                         size_t i) {
    return _tuple_index<0>(tpl, i);
}
template <typename T0, typename... Ts>
std::ostream& operator<<(std::ostream& s, std::variant<T0, Ts...> const& v) {
    std::visit([&](auto&& x) { s << x; }, v);
    return s;
}
// ===========

int main(int argc, char const* argv[]) {
    const std::tuple student = std::make_tuple("zsh", 18, "男");
    std::cout << std::get<0>(student) << "\n";
    std::cout << std::get<1>(student) << "\n";
    std::cout << std::get<2>(student) << "\n";
    // std::tie(std::string name, int age, std::string sex) = student;
    const auto& [name, age, sex] = student;
    std::cout << name << age << sex << "\n";
    return 0;
}

// https://changkun.de/modern-cpp/zh-cn/05-pointers/