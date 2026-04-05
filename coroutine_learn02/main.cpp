#include <coroutine>
#include <iostream>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

struct Awaitable {
    boost::asio::io_context& io;

    Awaitable(boost::asio::io_context& io) : io(io) {}

    bool await_ready() const noexcept { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
        auto timer = std::make_shared<boost::asio::steady_timer>(io, std::chrono::seconds(5));
        timer->async_wait([handle, timer](const boost::system::error_code&) {
            handle.resume();
        });
    }

    void await_resume() const noexcept {
        std::cout << "await_resume called, thread id: " << std::this_thread::get_id() << "\n";
    }
};

// 普通 C++ 协程 Task
struct Task {
    struct promise_type {
        Task get_return_object() { return Task{ std::coroutine_handle<promise_type>::from_promise(*this) }; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
    std::coroutine_handle<promise_type> handle;
};

Task helloCoroutine(boost::asio::io_context& io) {
    std::cout << "before await, thread id: " << std::this_thread::get_id() << "\n";
    co_await Awaitable(io);
    std::cout << "after await, thread id: " << std::this_thread::get_id() << "\n";
}

int main() {
    boost::asio::io_context io;
    auto t = helloCoroutine(io); // 启动协程
    io.run();
}
