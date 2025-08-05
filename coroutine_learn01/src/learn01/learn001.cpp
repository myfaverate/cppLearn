#include "learn001.hpp"
IntReader::IntReader() : value(0) {
    std::cout << "IntReader constructed with initial value: " << value << std::endl;
}
IntReader::~IntReader() {
    std::cout << "IntReader destructed" << std::endl;
}
bool IntReader::await_ready() const noexcept {
    return false;  // Always not ready, to simulate asynchronous behavior
}
void IntReader::await_suspend(std::coroutine_handle<> handle) noexcept {
    std::cout << "IntReader: suspending coroutine" << std::endl;
    std::thread thread([this, handle]() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate asynchronous operation
        std::cout << "IntReader: resuming coroutine" << std::endl;
        this->value = std::rand() % 100;  // Simulate reading an integer
        handle.resume();  // Resume the coroutine
        std::cout << "thread Id: " << std::this_thread::get_id() << std::endl;
    });
    thread.detach();  // Detach the thread to run independently
    std::cout << "await_suspend thread Id: " << std::this_thread::get_id() << std::endl;
}
int IntReader::await_resume() noexcept {
    std::cout << "IntReader: resuming with value: " << value << std::endl;
    return value;  // Return the read integer value
}
class Task {
public:
    class promise_type {
    public:
        Task get_return_object() {
            return {};
        }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};
Task PrintInt(){
    IntReader reader1;
    int total = co_await reader1;  // Co-await the IntReader
    std::cout << "Total from IntReader: " << total << std::endl;
    IntReader reader2;
    total += co_await reader2;  // Co-await another IntReader   
    std::cout << "Total from IntReaders: " << total << std::endl;
    IntReader reader3;
    total += co_await reader3;  // Co-await another IntReader   
    std::cout << "Total from IntReaders: " << total << std::endl;
}
void coroutineHello() {
    PrintInt();
    std::cout << "coroutineHello thread Id: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
}