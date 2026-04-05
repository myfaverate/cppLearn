#include "learn001.hpp"

/*
    g++ -E main.cpp -o main.ii
    g++ -S main.ii -o main.s
*/
struct HelloCoroutine{
public:
    struct promise_type{
        promise_type(){
            std::osyncstream(std::cout) << "promise_type constructed\n";
        }
        ~promise_type(){
            std::osyncstream(std::cout) << "promise_type destructed\n";
        }
        HelloCoroutine get_return_object(){
            std::osyncstream(std::cout) << "get_return_object\n";
            return HelloCoroutine(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        std::suspend_always initial_suspend(){
            std::osyncstream(std::cout) << "initial_suspend\n";
            return {};
        }
        std::suspend_always final_suspend() noexcept { // 1
            std::osyncstream(std::cout) << "final_suspend\n";
            return {};
        }
        void return_void(){
            std::osyncstream(std::cout) << "return_void\n";
        }
        void unhandled_exception(){
            std::osyncstream(std::cout) << "Coroutine encountered an exception.\n";
        }
    };
    void resume(){
        if(!handle.done()){
            handle.resume(); // 2
        }
    }
    HelloCoroutine(std::coroutine_handle<promise_type> coroutineHandle) : handle(coroutineHandle) {
        std::osyncstream(std::cout) << "HelloCoroutine constructed\n";
    }
    ~HelloCoroutine() {
        std::osyncstream(std::cout) << "HelloCoroutine destructed\n";
	    if(handle != nullptr){
            handle.destroy(); // 3
        }
    }
    auto getHandle() const {
        return handle;
    }
private:
    std::coroutine_handle<promise_type> handle = nullptr; // 指向协程帧
};

class Person{
private:
    std::coroutine_handle<HelloCoroutine::promise_type> callbackHandle;
public:
    Person() {
        std::osyncstream(std::cout) << "Person constructed\n";
    }
    ~Person() {
        std::osyncstream(std::cout) << "Person destructed\n";
    }
    void setCallbackHandle(std::coroutine_handle<HelloCoroutine::promise_type> handle){
        callbackHandle = handle;
    }
    auto getCallbackHandle() const {
        return callbackHandle;
    }
    void hello(std::function<void()> callback){
        std::thread t([callback]() {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟耗时任务
            callback();
        });
        t.detach();
    }
};

struct CallbackAwaitable{
private:
    Person &person;
public:
    CallbackAwaitable(Person& person) : person(person) {}
    bool await_ready() const noexcept{
        std::osyncstream(std::cout) << "CallbackAwaitable await_ready...\n";
        return false; // 挂起
    }
    void await_suspend(std::coroutine_handle<HelloCoroutine::promise_type> handle) noexcept {
        person.setCallbackHandle(handle);
    }
    void await_resume() const noexcept {
        std::osyncstream(std::cout) << "CallbackAwaitable await_resume\n";
    }
};


HelloCoroutine helloCoroutine(Person &person){
    std::osyncstream(std::cout) << "Hello, Coroutine! (Part 1: About to wait)" << std::endl;
    
    // 协程在这里暂停，并将控制权返回给调用者 (learn01::hello)
    co_await CallbackAwaitable(person);
    
    std::osyncstream(std::cout) << "Hello, Coroutine! (Part 2: Resumed by callback)" << std::endl;
    co_return;
}
namespace learn01 {
void list(std::filesystem::path p){
    std::queue<std::filesystem::path> dirs;
    dirs.push(p);
    while(!dirs.empty()){
        auto current = dirs.front();
        dirs.pop();
        for(const auto& entry : std::filesystem::directory_iterator(current)){
            if(entry.is_directory()){
                std::cout << entry.path().generic_string() << "\n";
                dirs.push(entry.path());
            } else {
                std::osyncstream(std::cout) << entry.path().generic_string() << "\n";
            }
        }
    }

}
void hello() {
    // std::filesystem::path p = std::filesystem::current_path().parent_path();
    std::filesystem::path p = std::filesystem::path("D:/SoftWare/LanguageProjects/C++Projects/learn02/include");
    std::osyncstream(std::cout) << "Current path: " << p.generic_string() << "\n";
    list(p);
}
}  // namespace learn01
