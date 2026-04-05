#include "learn001.hpp"

namespace learn01 {
    HelloCoroutine::HelloCoroutine(std::coroutine_handle<promise_type> handle) : handle(handle) {
         std::osyncstream(std::cout) << "HelloCoroutine constructed\n";
    }
    HelloCoroutine::~HelloCoroutine() {
         std::osyncstream(std::cout) << "HelloCoroutine destructed\n";
        if (handle) {
            handle.destroy();
        }
    }
    HelloCoroutine::promise_type::promise_type(){
         std::osyncstream(std::cout) << "promise_type constructed\n";
    }
    HelloCoroutine::promise_type::~promise_type(){
         std::osyncstream(std::cout) << "promise_type destructed\n";
    }
    HelloCoroutine HelloCoroutine::promise_type::get_return_object() {
         std::osyncstream(std::cout) << "get_return_object called\n";
        return HelloCoroutine{
            std::coroutine_handle<promise_type>::from_promise(*this)
        };
    }
    std::suspend_never HelloCoroutine::promise_type::initial_suspend() {
         std::osyncstream(std::cout) << "initial_suspend called before\n";
         std::osyncstream(std::cout) << "initial_suspend called after\n";
        return {};
    }
    std::suspend_never HelloCoroutine::promise_type::final_suspend() noexcept {
         std::osyncstream(std::cout) << "final_suspend called\n";
        return {};
    }
    void HelloCoroutine::promise_type::return_void() {
         std::osyncstream(std::cout) << "return_void called\n";
    }
    void HelloCoroutine::promise_type::unhandled_exception() {
         std::osyncstream(std::cout) << "unhandled_exception called\n";
    }
    awaitable::awaitable(boost::asio::io_context& ioContext) : io(ioContext) {
        std::cout << "awaitable constructed\n";
    }
    awaitable::~awaitable() {
        std::cout << "awaitable destructed\n";
    }
    bool awaitable::await_ready() const noexcept { 
         std::osyncstream(std::cout) << "awaitable await_ready called\n";
        return false; 
    }
    void awaitable::await_suspend(std::coroutine_handle<> handle) const noexcept {
        std::osyncstream(std::cout) << "awaitable await_suspend called before\n";
        std::thread([this, handle]{
            std::this_thread::sleep_for(std::chrono::seconds(1));
            boost::asio::post(io, [handle]() {
                std::osyncstream(std::cout) << "awaitable resuming coroutine in io_context thread\n";
                handle.resume();
            });
            std::cout << "sub thread id: " << std::this_thread::get_id() << "\n";
            std::osyncstream(std::cout) << "awaitable await_suspend called sub thread\n";
        }).detach();
        std::osyncstream(std::cout) << "awaitable await_suspend called after\n";
    }
    void awaitable::await_resume() const noexcept {
         std::osyncstream(std::cout) << "awaitable await_resume called\n";
    }
    void HelloCoroutine::resume(){
        if (handle && !handle.done()) {
             std::osyncstream(std::cout) << "resume...\n";
            handle.resume();
        }
    }
    struct Awaitable {
        boost::asio::io_context& io;
        Awaitable(boost::asio::io_context& io) : io(io) {}

        bool await_ready() const noexcept { return false; }

        void await_suspend(std::coroutine_handle<> handle) {
            std::thread([this, handle] {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                // 使用 free function post 调度协程恢复
                 std::cout << "sub thread id: " << std::this_thread::get_id() << "\n";
                boost::asio::post(io, [handle] { handle.resume(); });
            }).detach();  // 异步，不阻塞
        }

        void await_resume() const noexcept {
            std::cout << "await_resume called, thread id: "
                      << std::this_thread::get_id() << "\n";
        }
    };
    HelloCoroutine helloCoroutine(boost::asio::io_context& io) {
        std::osyncstream(std::cout) << "helloCoroutine called before\n";
        co_await Awaitable{io};  // 1
        std::osyncstream(std::cout) << "helloCoroutine called after\n";
        co_return;
    }
    void hello() {
        std::cout << "main thread id: " << std::this_thread::get_id() << "\n";
        boost::asio::io_context ioContext(1);
        auto hello = helloCoroutine(ioContext);
        ioContext.run();
    }
}