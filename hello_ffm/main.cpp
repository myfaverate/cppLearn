#include <iostream>

extern "C" __declspec(dllexport)
void sayHello(){
    std::cout << "Hello, from hello_ffm!\n";
}

extern "C" __declspec(dllexport)
const char* getString(){
    return "Hello, from getString!";
}