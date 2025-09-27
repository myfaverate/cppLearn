#include "learn021.hpp"

struct A;
struct B;

struct A {
    std::shared_ptr<B> bptr;
    void func(){
        std::cout << "A::func()\n";
    }
    A(){
        std::cout << "A()\n";
    }
    ~A(){
        std::cout << "~A()\n";
    }
};
struct B {
    std::weak_ptr<A> aptr;
    B(){
        std::cout << "B()\n";
    }
    ~B(){
        std::cout << "~B()\n";
    }
};
template<typename T>
void printBit(T value) {
    for (int i = sizeof(T) * 8 - 1; i >= 0; --i) {
        std::cout << ((value >> i) & 1);
        if (i % 8 == 0) std::cout << ' ';
    }
    std::cout << '\n';
}
void learn021() {
    int a = 100000;
    char b = a;
    std::cout << b << "\n";
}