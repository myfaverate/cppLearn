#include "learn01.hpp"

// 一个简单的协程返回类型
struct HelloTask {
    struct promise_type {
        HelloTask get_return_object() { 
            return HelloTask{ std::coroutine_handle<promise_type>::from_promise(*this) }; 
        }
        std::suspend_never initial_suspend() { return {}; }   // 直接开始执行
        std::suspend_never final_suspend() noexcept { return {}; } // 结束后不挂起
        void return_void() {}
        void unhandled_exception() { std::exit(1); }
    };

    std::coroutine_handle<promise_type> handle;

    HelloTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~HelloTask() { if (handle) handle.destroy(); }
};

// 协程函数
HelloTask hello() {
    std::cout << "Hello from coroutine!" << std::endl;
    co_return;
}

/**
 * https://zplutor.github.io/2022/03/25/cpp-coroutine-beginner/
 * https://www.bennyhuo.com/book/cpp-coroutines/00-foreword.html
 */
void message() {
    auto task = hello();
    std::cout << "add..\n";
}