#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <coroutine>

#include "Person.hpp"
int main() {
    std::ifstream file("output.txt", std::ios::binary);
    if (file.is_open()) {
        // std::string line;
        // while (std::getline(file, line)) {
        //     std::cout << line << std::endl;
        // }

        constexpr int bufferSize = 100;
        std::vector<char> buffer(bufferSize);
        while (file.read(buffer.data(), buffer.size())) {
            std::cout.write(buffer.data(), file.gcount());
        }
        // 处理不足 1KB 的最后一段
        std::streamsize bytesRead = file.gcount();
        std::cout << "\nbytesRead: " << bytesRead << std::endl;
        if (bytesRead > 0) {
            std::cout.write(buffer.data(), bytesRead);
        }
        file.close();
        std::cout << "File read successfully." << std::endl;
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }
    Person person("Alice", 30);
    std::cout << person << std::endl;
    return 0;
}