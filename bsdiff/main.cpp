#include "HelloWorld.hpp"

#include <iostream>


int main(){
    create_patch("D:/SoftWare/LanguageProjects/C++Projects/bsdiff/Hello.txt", "D:/SoftWare/LanguageProjects/C++Projects/bsdiff/Hello2.txt", "patch.bin");
    apply_patch("D:/SoftWare/LanguageProjects/C++Projects/bsdiff/Hello.txt", "D:/SoftWare/LanguageProjects/C++Projects/bsdiff/HelloTmp.txt", "D:/SoftWare/LanguageProjects/C++Projects/bsdiff/build/patch.bin");
    std::cout << "Hello, from bsdiff!\n";
    return 0;
}
