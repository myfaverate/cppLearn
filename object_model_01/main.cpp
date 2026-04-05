#include <iostream>
#include <initializer_list>
#include <cassert>
auto main(int, const char**) -> int {
#ifdef NDEBUG
    std::cout << "NDEBUG is defined, assert() will not be evaluated." << std::endl;
#else
    std::cout << "NDEBUG is not defined, assert() will be evaluated." << std::endl;
#endif
    std::cout << "file: " << __FILE__ << std::endl;
    std::cout << "function: " << __func__ << std::endl;
    std::cout << "line: " << __LINE__ << std::endl;
    std::cout << "time: " << __TIME__ << std::endl;
    std::cout << "date: " << __DATE__ << std::endl;
    assert(2 + 2 == 4); // This assertion will pass.
    assert(2 + 2 == 5); // This assertion will fail and terminate the
    return 0;
}