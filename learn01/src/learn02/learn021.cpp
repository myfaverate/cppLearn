#include "learn021.hpp"

namespace learn021 {
    class awaitable {
    public:
        awaitable(){
            std::cout << "awaitable constructed\n";
        } 
        ~awaitable(){
            std::cout << "awaitable destructed\n";
        }
        bool await_ready() const noexcept { 
            std::cout << "await_ready\n";
            return false;
         }
        void await_suspend(std::coroutine_handle<> h) const noexcept {
            std::cout << "await_suspend\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            h.resume();
        }
        void await_resume() const noexcept {
            std::cout << "await_resume\n";
        }
    };
    class HelloCoroutine {
    public:
        class promise_type {
        public:
            promise_type(){
                std::cout << "promise_type constructed1\n";
            }
            ~promise_type(){
                std::cout << "promise_type destructed\n";
            }
            HelloCoroutine get_return_object(){
                std::cout << "get_return_object\n";
                return HelloCoroutine(std::coroutine_handle<promise_type>::from_promise(*this));  
            }
            std::suspend_always initial_suspend(){
                std::cout << "initial_suspend\n";
                return {};
            }
            std::suspend_always final_suspend() noexcept { // 1
                std::cout << "final_suspend\n";
                return {};
            }
            void return_void(){
                std::cout << "return_void\n";
            }
            void unhandled_exception(){
                std::cerr << "Coroutine encountered an exception.\n";
            }
        };
        HelloCoroutine(std::coroutine_handle<promise_type> coroutineHandle) : handle(coroutineHandle) {
            std::cout << "HelloCoroutine constructed\n";
        }
        ~HelloCoroutine() {
            std::cout << "HelloCoroutine destructed\n";
            handle.destroy(); // 3
        }
        void resume(){
            if(!handle.done()){
                handle.resume(); // 2
            }
        }
    private:
        std::coroutine_handle<promise_type> handle; // 指向协程帧
    };

    HelloCoroutine helloCoroutine(){
        std::cout << "Hello, Coroutine!" << std::endl;
        co_await awaitable{};
        co_return;
    }
    void learn01(){
        HelloCoroutine hello = helloCoroutine();
        hello.resume();
        std::cout << "This is learn01 function." << std::endl;
    }

    static boost::asio::awaitable<std::string> async_print(const std::string message){
        std::cout << "message1: " << message << "\n";
        auto executor = co_await boost::asio::this_coro::executor;
        std::cout << "message2: " << message << "\n";
        boost::asio::steady_timer timer(executor);
        timer.expires_after(std::chrono::seconds(1));
        std::cout << "message3: " << message << "\n";
        co_await timer.async_wait(boost::asio::use_awaitable);
        std::cout << "message4: " << message << "\n";
        const auto threadId = std::this_thread::get_id();
        std::cout << "threadId: " << threadId << "\n";
        co_return "Hello World Coroutine!";
    }
    void learn02(){
        std::cout << "hello World02n\n";
        boost::asio::io_context ioContext(1);
        std::vector<std::future<std::string>> results;
        constexpr u_int N = 10;
        for(u_int i = 0; i < N; i ++ ){
            results.emplace_back(boost::asio::co_spawn(ioContext, async_print("Hello World From Coroutine!"), boost::asio::use_future));
        }
        ioContext.run();
        for(u_int i = 0; i < N; i ++ ){
            std::cout << "result i: " << i << ", value: " << results[i].get() << "\n";
        }
    }
}