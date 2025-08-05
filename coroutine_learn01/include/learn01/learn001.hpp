#ifndef COROUTINE_LEARN001_HPP
#define COROUTINE_LEARN001_HPP

#include <iostream>
#include <coroutine>
#include <thread>

class IntReader{
public:
    IntReader();  // 构造函数，初始化整数值为0
    ~IntReader();  // 默认析构函数

    // bool await_ready() const noexcept {
    //     return false;  // 立即准备好
    // }
    // void await_suspend(std::coroutine_handle<> handle) noexcept {
    //     // 挂起协程
    //     std::cout << "IntReader: suspending coroutine" << std::endl;
    //     std::thread thread([this, handle] -> void {
    //         std::srand(static_cast<unsigned int>(std::time(nullptr)));
    //         std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟异步操作
    //         std::cout << "IntReader: resuming coroutine" << std::endl;
    //         this->value = std::rand() % 100;  // 模拟异步读取整数
    //         handle.resume();  // 恢复协程
    //     })
    //     thread.detach();  // 分离线程
    // }
    bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    int await_resume() noexcept;
private:
    int value;  // 存储读取的整数
};

void coroutineHello();

#endif // COROUTINE_LEARN001_HPP