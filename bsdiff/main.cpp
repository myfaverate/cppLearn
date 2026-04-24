#include "HelloWorld.hpp"

#include <iostream>

int main(){
    // create_patch("/Users/zsh/Projects/CppProjects/bsdiff/hello1.txt", "/Users/zsh/Projects/CppProjects/bsdiff/hello2.txt", "/Users/zsh/Projects/CppProjects/bsdiff/hello.patch");
    apply_patch("/Users/zsh/Projects/CppProjects/bsdiff/hello1.txt", "/Users/zsh/Projects/CppProjects/bsdiff/hello2.txt", "/Users/zsh/Projects/CppProjects/bsdiff/hello.patch");
    std::cout << "Hello, from bsdiff!\n";

    return 0;
}
// https\://mirrors.cloud.tencent.com/gradle