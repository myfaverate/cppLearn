#ifndef LEARN001_HPP
#define LEARN001_HPP

#include <coroutine>
#include <iostream>
#include <thread>
#include <chrono>
#include <syncstream>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>

namespace learn01 {

struct HelloCoroutine {
public:
    struct awaitable;
    struct promise_type {
        promise_type();
        ~promise_type();
        HelloCoroutine get_return_object();
        std::suspend_never initial_suspend();
        std::suspend_never final_suspend() noexcept;
        void return_void();
        void unhandled_exception();
    };
    HelloCoroutine(std::coroutine_handle<promise_type> handle);
    ~HelloCoroutine();
    void resume();
private:
    std::coroutine_handle<promise_type> handle;
};

struct awaitable {
private:
    boost::asio::io_context& io;
public:
    awaitable(boost::asio::io_context& ioContext);
    ~awaitable();
    bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<> handle) const noexcept;
    void await_resume() const noexcept;
};

HelloCoroutine helloCoroutine(boost::asio::io_context& io);

void hello();
}  // namespace learn01

#endif  // LEARN001_HPP