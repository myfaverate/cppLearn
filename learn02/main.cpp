#include <array>
#include <chrono>
#include <cstring>
#include <format>
#include <forward_list>
#include <list>
#include <sstream>
#include <thread>
#include <vector>
#include <print>

#include "learn001.hpp"

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// class Person {
//    private:
//     std::string name;

//    public:
//     // 构造函数
//     Person(const std::string& name) : name(name) {
//         std::cout << "Person constructed: " << name << std::endl;
//     }
//     // 析构函数
//     ~Person() { std::cout << "Person destructed: " << name << std::endl; }
//     // 拷贝构造函数
//     Person(const Person& other) : name(other.name) {
//         std::cout << "Person copy-constructed: " << name << std::endl;
//     }
//     // 移动构造函数
//     Person(Person&& other) noexcept : name(std::move(other.name)) {
//         std::cout << "Person move-constructed: " << name << std::endl;
//     }
//     // 赋值运算符
//     Person& operator=(const Person& other) {
//         if (this != &other) {
//             name = other.name;
//             std::cout << "Person copy-assigned: " << name << std::endl;
//         }
//         return *this;
//     }
//     // 移动赋值运算符
//     Person& operator=(Person&& other) noexcept {
//         if (this != &other) {
//             name = std::move(other.name);
//             std::cout << "Person move-assigned: " << name << std::endl;
//         }
//         return *this;
//     }
//     const std::string& getName() const { return name; }
//     void setName(const std::string& name) { this->name = name; }
//     friend std::ostream& operator<<(std::ostream& os, const Person& person) {
//         os << "Person(Name=" << person.name << ")";
//         return os;
//     }
// };

// TODO
// #include <variant>
// template <size_t n, typename... T>
// constexpr std::variant<T...> _tuple_index(const std::tuple<T...>& tpl, size_t i) {
//     if constexpr (n >= sizeof...(T)) throw std::out_of_range("越界.");
//     if (i == n)
//         return std::variant<T...>{std::in_place_index<n>, std::get<n>(tpl)};
//     return _tuple_index<(n < sizeof...(T) - 1 ? n + 1 : 0)>(tpl, i);
// }
// template <typename... T>
// constexpr std::variant<T...> tuple_index(const std::tuple<T...>& tpl,
//                                          size_t i) {
//     return _tuple_index<0>(tpl, i);
// }
// template <typename T0, typename... Ts>
// std::ostream& operator<<(std::ostream& s, std::variant<T0, Ts...> const& v) {
//     std::visit([&](auto&& x) { s << x; }, v);
//     return s;
// }
class User{
public:
    std::string name;
    uint32_t age;
    std::string gender;
    User(const std::string name, const uint32_t age, const std::string gender) : name(name), age(age), gender(gender){
        std::cout << "构造器\n";
    }
};
template <> // 特化模板
struct std::formatter<User> { // 必须在std命名空间
    // 1. parse 方法：无需自定义格式，直接返回迭代器（兼容 C++20+）
    constexpr auto parse(const std::format_parse_context& ctx) {
        return ctx.begin();  // 不解析自定义格式符，直接使用固定格式
    }

    // 2. format 方法：拼接所有成员，生成结构化字符串
    auto format(const User& user, std::format_context& ctx) const {
        // 使用 std::format_to 写入格式化结果到 ctx.out()
        return std::format_to(
            ctx.out(),
            R"(User(name={}, age={}, gender={}))",  // {{ 和 }} 转义为 { 和 }
            user.name,
            user.age,
            user.gender
        );
    }
};

// ===========
// Person getPerson(){
//     Person person("zsh");
//     std::cout << &person << "\n";
//     return person;
// }

template<class T>
T add(T a, T b){
	return a + b;
}
void main1(){
  	std::cout << add(1, 2) << "\n";
  	std::cout << add(std::string("1"), std::string("2")) << "\n";
	std::cout << "Hello World\n";
}

using namespace std::string_literals;

template<typename T1, typename T2, typename RT = decltype(true ? T1{} : T2{})>
RT max(const T1& a, const T2& b) { // RT 是 std::string
    return a > b ? a : b;
}

template<typename T, typename T2>
auto max1(const T& a, const T2& b) -> decltype(true ? a : b) {
    return a > b ? a : b;
}
template<typename T, typename T2>
decltype(auto) max2(const T& a, const T2& b){
    return a > b ? a : b;
}

template<const std::size_t N = 1000>
void f() {
    std::cout << N << '\n';
}

void f(const char*, int, double) { puts("值"); }
void f(const char**, int*, double*) { puts("&"); }

template<typename... Args>
void sum(Args... args){  // const char * args0, int args1, double args2
    f(args...);   // 相当于 f(args0, args1, args2)
    f(&args...);  // 相当于 f(&args0, &args1, &args2)
}
template<typename... Args>
void print(const Args... args){    // const char (&args0)[5], const int & args1, const double & args2
    int a[]{ (std::cout << args << ' ' ,0)... };
}
template<typename... Args>
void print1(const Args&... args) {
    using Arr = int[]; // 创建临时数组，需要使用别名
    Arr{ 0, (std::cout << args << ' ' ,0)... };
}

#include <type_traits>
template<typename... Args, typename RT = std::common_type_t<Args...>>
RT sum1(const Args&... args) {
    RT _[]{ static_cast<RT>(args)... };
    RT n{};
    for (int i = 0; i < sizeof...(args); ++i) {
        n += _[i];
    }
    return n;
}
#include <iostream>
template<typename T0>
void printf1(T0 value) {
    std::cout << "value2: " << value << std::endl;
}
template<typename T, typename... Ts>
void printf1(T value, Ts... args) {
    std::cout << "value1: " << value << std::endl;
    printf1(args...);
}
template<typename T0, typename... T>
void printf2(T0 t0, T... t) {
    std::cout << "value3: " << t0 << std::endl;
    if constexpr (sizeof...(t) > 0) printf2(t...);
}
template <typename T, typename... Ts>
auto printf3(T value, Ts... args) {
    std::cout << "value41: " << value << std::endl;
    const auto datas = std::initializer_list<T> {
        ([&args] -> void { 
            std::cout << "value42: " << args << std::endl; 
        }(), value)...
    };
    for(const auto &data : datas){
        std::cout << "data: " << data << "\n";
    }
}

template<typename... T>
void print4(T... args){
    (std::cout << ... << args);
}

int main(int argc, char const* argv[]) {
    learn01::hello();
    return 0;
}
// https://mq-b.github.io/Modern-Cpp-templates-tutorial/md/%E7%AC%AC%E4%B8%80%E9%83%A8%E5%88%86-%E5%9F%BA%E7%A1%80%E7%9F%A5%E8%AF%86/02%E7%B1%BB%E6%A8%A1%E6%9D%BF
// https://cppinsights.io/
// https://github.com/Mq-b/Modern-Cpp-templates-tutorial
// https://changkun.de/modern-cpp/zh-cn/02-usability/#%E5%8F%98%E9%95%BF%E5%8F%82%E6%95%B0%E6%A8%A1%E6%9D%BF
// https://changkun.de/modern-cpp/zh-cn/05-pointers/