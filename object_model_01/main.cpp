#include <iostream>

#include "sha256sum.hpp"
#include "startup_demo.hpp"

auto main(int argc, char** argv) -> int {
    if (argc >= 2) {
        try {
            std::cout << sha256sum::hash_file(argv[1]) << '\n';
            return 0;
        } catch (const std::exception& e) {
            std::cerr << "error: " << e.what() << '\n';
            return 1;
        }
    }
    startup_demo::run();
}