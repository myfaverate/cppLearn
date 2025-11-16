#include "learn001.hpp"

const char * program = "D:/SoftWare/LanguageProjects/C++Projects/epoll_learn01/build/epoll_learn01.exe";

void hello00(){
    std::cout << "before Hello hello00\n";
    const char * const args[] = {program, "hello01", nullptr};
    const auto result = execv(program, const_cast<char**>(args));
    std::cout << "after Hello hello00 execv result: " << result << "\n";
}


void hello01(){
    std::cout << "before Hello hello01\n";
    const char * const args[] = {program, "hello02", nullptr};
    const auto result = execv(program, const_cast<char**>(args));
    std::cout << "after Hello hello01 execv result: " << result << "\n";
}

void hello02(){
    std::cout << "before Hello hello02\n";
    const char * const args[] = {program, "hello03", nullptr};
    const auto result = execv(program, const_cast<char**>(args));
    std::cout << "after Hello hello02 execv result: " << result << "\n";
}

void hello03(){
    std::cout << "Hello hello03\n";
}
// C code
int add(int x, int y) {
    return x+y;
}
int main(int, char**){
    // use lambda expression
    [out = std::ref(std::cout << "Result from C code: " << add(1, 2))]() -> void {
        out.get() << ".\n";
    }();
    return 0;
}