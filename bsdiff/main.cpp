
#include "HelloWorld.hpp"

#include <iostream>


auto main() -> int {
    // create_patch("../hello1.txt", "../hello2.txt", "../hello.patch");
    apply_patch("../hello1.txt", "../hello2_restored.txt", "../hello.patch");    
    return 0;
}