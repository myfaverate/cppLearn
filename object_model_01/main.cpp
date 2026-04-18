
#include <iostream>

auto main(const int argc, const char** const argv) -> int { 
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }
    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << line << std::endl;
    }
    return 0; 
}
// https://www.bilibili.com/video/BV13Q4y1C7hS?spm_id_from=333.788.player.switch&vd_source=fb5fc0881b2bb1a411566e5b2f1c7c7e&p=115