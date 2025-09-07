#include "learn02.hpp"

void learn02(){
    std::cout << "This is learn02 function." << std::endl;
    std::vector<int> vec = {1, 2, 3, 4, 5, 6};
    int middle = vec.size() / 2;
    for(int i = 1; i <= middle; ++i){
        // std::cout << vec[i] << " " << vec[vec.size() - 1 - i] << "\n";
        vec[i - 1] = vec[i];
        vec[vec.size() - i] = vec[vec.size() - 1 - i];
    }
    for(int i : vec){
        std::cout << i << " ";
    }
    std::cout << "\n";
}
struct HelloCoroutine {
public:
    struct promise_type {
    public:
        promise_type(){
            std::cout << "promise_type constructed\n";
        }
        ~promise_type(){
            std::cout << "promise_type destructed\n";
        }
        HelloCoroutine get_return_object(){
            std::cout << "get_return_object\n";
            return HelloCoroutine(std::coroutine_handle<promise_type>::from_promise(*this)); // 解引用
        }
        std::suspend_always initial_suspend(){
            std::cout << "initial_suspend\n";
            return {};
        }
        std::suspend_always final_suspend() noexcept {
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
    struct awaitable {
    private:
        int a, b;
    public:
        std::promise<int> promise;
        awaitable(const int a, const int b) : a(a), b(b) {
            std::cout << "awaitable constructed\n";
        }
        ~awaitable(){
            std::cout << "awaitable destructed\n";
        }
        bool await_ready() const noexcept { 
            std::cout << "await_ready\n";
            return false;
        }
        void await_suspend(std::coroutine_handle<promise_type> h) noexcept {
            std::cout << "await_suspend\n";
            // 在后台线程中延迟后恢复协程
            std::thread([this, h, delay = 1000]() -> void {
                std::cout << "Starting background computation in thread id: " << std::this_thread::get_id() << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // []()->void::delay delay 是闭包当中的一个属性
                std::cout << "计算完成，恢复协程\n";
                promise.set_value(a + b);
                h.resume(); // 自动恢复协程
            }).detach(); // 分离线程，使其在后台运行
        }
        std::future<int> await_resume() noexcept {
            std::cout << "await_resume\n";
            std::cout << "await_resume Starting background computation in thread id: " << std::this_thread::get_id() << "\n";
            return promise.get_future(); // 获取结果，阻塞等待
        }
    };
    HelloCoroutine(std::coroutine_handle<promise_type> coroutineHandle) : handle(coroutineHandle) {
        std::cout << "HelloCoroutine constructed\n";
    }
    ~HelloCoroutine() {
        std::cout << "HelloCoroutine destructed\n";
        handle.destroy();
    }
    void resume() {
        if (!handle.done()) {
            handle.resume();
            std::cout << "Coroutine resumed\n";
        }
    }
private:
    std::coroutine_handle<promise_type> handle; // 指向协程帧
};
HelloCoroutine helloCoroutine(){
    std::cout << "Hello, Coroutine before!" << std::endl;
    auto result1 = co_await HelloCoroutine::awaitable(1, 2);
    auto result2 = co_await HelloCoroutine::awaitable(3, 4);
    std::cout << "Coroutine results: " << result1.get() << ", " << result2.get() << std::endl;
    std::cout << "Coroutine returned value thread id: " << std::this_thread::get_id()<< std::endl;
    co_return;
}
void learn01(){
    HelloCoroutine hello = helloCoroutine();
    hello.resume(); // 开启协程
    std::cout << "Coroutine returned value thread id: " << std::this_thread::get_id()<< std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); 
}

// HelloCoroutine helloCoroutine1(){
//     coroutine_frame *frame = new coroutine_frame();
//     HelloCoroutine::promise_type &promise = frame->promise;
//     HelloCoroutine return_object = promise.get_return_object(); // init HelloCoroutine
//     co_await promise.initial_suspend();
//     try{
//         std::cout << "Hello, Coroutine!" << std::endl;
//         promise.return_void(); // co_return;
//     }catch(...){
//         promise.unhandled_exception();
//     }
//     co_await promise.final_suspend();
// }

/*
#include <iostream>
#include <string>
#include <asio.hpp>   // 如果没有独立的 asio，用 <boost/asio.hpp>

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
        std::cout << "Server running on http://127.0.0.1:8080\n";

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            // 读取 HTTP 请求
            char buffer[1024];
            std::error_code ec;
            size_t length = socket.read_some(asio::buffer(buffer), ec);

            if (!ec) {
                std::string request(buffer, length);
                std::cout << "Request:\n" << request << std::endl;

                // 构造 HTTP 响应
                std::string response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: 11\r\n"
                    "\r\n"
                    "Hello World";

                asio::write(socket, asio::buffer(response), ec);
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

*/
using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
awaitable<void> handle_client(tcp::socket socket) {
    try {
        char buffer[1024];
        while(true){
            auto handleClientThreadId1 = std::this_thread::get_id();
            std::cout << "handleClientThreadId1: " << handleClientThreadId1 << "\n";
            std::size_t n = co_await socket.async_read_some(boost::asio::buffer(buffer), use_awaitable);
            std::string request(buffer, n);
            std::cout << "Request:\n" << request << "\n";
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 11\r\n"
                "\r\n"
                "Hello World";
            auto handleClientThreadId2 = std::this_thread::get_id();
            std::cout << "handleClientThreadId2: " << handleClientThreadId2 << "\n";
            co_await boost::asio::async_write(socket, boost::asio::buffer(response), use_awaitable);
            auto handleClientThreadId3 = std::this_thread::get_id();
            std::cout << "handleClientThreadId3: " << handleClientThreadId3 << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Client disconnected: " << e.what() << "\n";
    }
}
awaitable<void> listener(tcp::endpoint endpoint) {
    auto listenerThreadId1 = std::this_thread::get_id();
    std::cout << "listenerThreadId1: " << listenerThreadId1 << "\n";
    auto executor = co_await boost::asio::this_coro::executor;
    auto listenerThreadId2 = std::this_thread::get_id();
    std::cout << "listenerThreadId2: " << listenerThreadId2 << "\n";
    tcp::acceptor acceptor(executor, endpoint);
    while (true) {
        auto listenerThreadId3 = std::this_thread::get_id();
        std::cout << "listenerThreadId3: " << listenerThreadId3 << "\n";
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        auto listenerThreadId4 = std::this_thread::get_id();
        std::cout << "listenerThreadId4: " << listenerThreadId4 << "\n";
        co_spawn(executor, handle_client(std::move(socket)), detached);
    }
}
void learn03(){
    // try {
    //     boost::asio::io_context ioContext;
    //     tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 8080));
    //     while(true){
    //         tcp::socket socket(ioContext);
    //         acceptor.accept(socket);
    //         char buffer[1024];
    //         boost::system::error_code errorCode;
    //         auto length = socket.read_some(boost::asio::buffer(buffer), errorCode);
    //         if(!errorCode.failed()){
    //             std::string request(buffer, length);
    //             std::cout << "request: \n" << request << "\n";

    //             std::string response =
    //                 "HTTP/1.1 200 OK\r\n"
    //                 "Content-Type: text/plain\r\n"
    //                 "Content-Length: 11\r\n"
    //                 "\r\n"
    //                 "Hello World";

    //             boost::asio::write(socket, boost::asio::buffer(response), errorCode);
    //         }
    //     }
    // }
    // catch(const std::exception& e) {
    //     std::cerr << e.what() << '\n';
    // }
    try {
        auto mainThreadId = std::this_thread::get_id();
        std::cout << "mainThreadId: " << mainThreadId << "\n";
        boost::asio::io_context io_context(1);
        co_spawn(io_context, listener(tcp::endpoint(tcp::v4(), 8080)), detached);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
struct Task {
public:
    struct promise_type {
        promise_type(){
            std::cout << "promise_type constructor\n";
        }
        ~promise_type(){
            std::cout << "promise_type destructor\n";
        }
        Task get_return_object() {
            std::cout << "get_return_object\n";
            return Task(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        std::suspend_always initial_suspend() { 
            std::cout << "initial_suspend\n";
            return {}; 
        }
        std::suspend_always final_suspend() noexcept { 
            std::cout << "final_suspend\n";
            return {}; 
        }
        void return_void() {
            std::cout << "return_void\n";
        }
        void unhandled_exception() { 
            std::cout << "unhandled_exception\n";
        }
    };
    void resume(){
        std::lock_guard<std::mutex> lock(mutex); // RAII
        if(!handle.done()){
            handle.resume(); // 2
        } // 临界区
    }
    Task(std::coroutine_handle<promise_type> coroutineHandle) : handle(coroutineHandle) {
        std::cout << "Task constructor\n";
    }
    ~Task(){
        handle.destroy();
        std::cout << "Task destructor\n";
    }
private:
    std::coroutine_handle<promise_type> handle; // 指向协程帧
    std::mutex mutex; // 保护多线程 resume
};

class Animal {
private:
    std::string species;
    int age;
public:
    // 默认构造函数
    Animal() : species("单细胞"), age(0){
        std::cout << "Animal Default Constructor" << "\n";
    }
    // 带参构造函数
    Animal(std::string species, int age) : species(species), age(age){
        std::cout << std::format("Animal Constructor species: {}, age: {}", species, age) << "\n";
    }
    // 拷贝构造函数
    Animal(const Animal& other) : species(other.species), age(other.age){
        std::cout << std::format("Animal Copy Constructor species: {}, age: {}", species, age) << "\n";
    }
    // 移动构造函数
    Animal(Animal&& other) noexcept : species(std::move(other.species)), age(std::exchange(other.age, 0)){
        std::cout << std::format("Animal Move Constructor species: {}, age: {}", species, age) << "\n";
    }
    // 析构函数
    virtual ~Animal(){
        std::cout << std::format("Animal Destructor1 species: {}, age: {}", species, age) << "\n";
        setAge(0);
        setSpecies("");
        std::cout << std::format("Animal Destructor2 species: {}, age: {}", species, age) << "\n";
    }
    // 重写赋值运算符
    Animal& operator=(const Animal& other){
        if(this != &other){
            species = other.species;
            age = other.age;
        }
        std::cout << std::format("Animal Copy Assignment Operator species: {}, age: {}", species, age) << "\n";
        return *this;
    }
    // 移动赋值运算符
    Animal& operator=(Animal&& other) noexcept {
        if(this != &other){
            species = std::move(other.species);
            age = std::exchange(other.age, 0);
        }
        std::cout << std::format("Animal Move Assignment Operator species: {}, age: {}", species, age) << "\n";
        return *this;   
    }
    // 重新<<运算符, 友元：访问成员私有变量
    friend std::ostream& operator<<(std::ostream& os, const Animal& animal) {
        os << std::format("species: {}, age: {}", animal.species, animal.age);
        return os;
    }
    // 定义为全局方法也是可以的
    // static std::ostream& operator<<(std::ostream& os, const Animal& animal) {
    //     os << std::format("species: {}, age: {}", animal.getSpecies(), animal.getAge());
    //     return os;
    // }

    /**
     * 第一个 const 代表返回不可变的值，一般和引用搭配
     * 第二个 const 代表这个函数无法修改this对象的任何属性
     */
    const std::string& getSpecies() const { 
        return species;
    }
    int getAge() const { 
        return age; 
    }
    void setSpecies(const std::string species) { 
        this->species = species; 
    }
    void setAge(const int age) { 
        this->age = age; 
    }
    // 虚函数，有虚函数，此对象仍然是一个正常可实例化的对象
    virtual void makeSound() const {
        std::cout << "Some generic animal sound" << std::endl;
    };
    virtual void eat(){
        std::cout << "Animal is eating" << std::endl;
    }
    virtual void sleep(){
        std::cout << "Animal is sleeping" << std::endl;
    }
};

Task coro(const std::string& msg) {
    std::cout << "animalPtr1: " << &msg << "\n";
    auto coroThreadId1 = std::this_thread::get_id();
    std::cout << "coroThreadId1: " << coroThreadId1 << "\n";
    std::cout << "before suspend: " << msg << "\n";
    co_await std::suspend_always{};   // <-- 挂起点
    std::cout << "animalPtr2: " << &msg << "\n";
    auto coroThreadId2 = std::this_thread::get_id();
    std::cout << "coroThreadId2: " << coroThreadId2 << "\n";
    std::cout << "middle1 suspend: " << msg << "\n"; // ⚠️ 这里可能悬空
    co_await std::suspend_always{};   // <-- 挂起点
    std::cout << "animalPtr3: " << &msg << "\n";
    auto coroThreadId3 = std::this_thread::get_id();
    std::cout << "coroThreadId3: " << coroThreadId3 << "\n";
    std::cout << "middle2 suspend: " << msg << "\n"; // ⚠️ 这里可能悬空
    co_await std::suspend_always{};   // <-- 挂起点
    std::cout << "after suspend: " << msg << "\n"; // ⚠️ 这里可能悬空
    auto coroThreadId4 = std::this_thread::get_id();
    std::cout << "coroThreadId4: " << coroThreadId4 << "\n";
    std::cout << "animalPtr4: " << &msg << "\n";
}

// 用 asio::post 包装任意耗时操作
boost::asio::awaitable<int> my_async_task(int value) {
    auto exec = co_await boost::asio::this_coro::executor;
    // post 一个任务到 io_context
    co_await boost::asio::post(exec, boost::asio::use_awaitable);
    auto threadId = std::this_thread::get_id();
    // std::this_thread::sleep_for(std::chrono::microseconds(100));
    std::cout << "running in io_context thread threadId: " << threadId << "\n";
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    co_return value * 2;
}
static awaitable<int> asyncReadFile(const std::string fileName){
    std::this_thread::sleep_for(std::chrono::microseconds(200));
    int result = co_await my_async_task(42);  // 自定义 Awaitable
    std::this_thread::sleep_for(std::chrono::microseconds(200));
    std::cout << "after co_await, result=" << result << "\n";
    std::this_thread::sleep_for(std::chrono::microseconds(200));
    co_return 1 + 2;
}
static constexpr u_int N = 10000;
void learn04(){
    auto cores = std::thread::hardware_concurrency();
    std::cout << "cores: " << cores << "\n"; 

    boost::asio::io_context ioContext;

    // 创建一个 work_guard 避免 io_context 过早退出
    auto work = boost::asio::make_work_guard(ioContext);

    // 启动线程池
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < cores; ++i){
        threads.emplace_back([&](){ ioContext.run(); });
    }

    // 提交协程
    auto result1 = boost::asio::co_spawn(ioContext, asyncReadFile("D:/SoftWare/LanguageProjects/C++Projects/learn01/src/learn02/learn02.cpp"), boost::asio::use_future);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    auto result2 = boost::asio::co_spawn(ioContext, asyncReadFile("D:/SoftWare/LanguageProjects/C++Projects/learn01/src/learn01/learn01.cpp"), boost::asio::use_future);

    // 等待结果
    std::cout << result1.get() << "\n";
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    std::cout << result2.get() << "\n";

    std::vector<std::future<int>> results;
    
    for(int i = 0; i < N; i ++ ){
        results.emplace_back(boost::asio::co_spawn(ioContext, asyncReadFile("D:/SoftWare/LanguageProjects/C++Projects/learn01/src/learn02/learn02.cpp"), boost::asio::use_future));
    }

    for(int i = 0; i < N; i ++ ){
        std::cout << "result: " << i << ": " <<  results[i].get() << "\n";
    }

    // 停止 io_context
    work.reset();
    for(auto& t : threads) t.join();
}