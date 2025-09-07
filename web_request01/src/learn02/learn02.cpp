#include "learn02.hpp"

namespace asio = boost::asio;
using asio::ip::tcp;
using asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;

awaitable<void>  async_get_request(){

    boost::asio::any_io_executor executor = co_await asio::this_coro::executor;

    tcp::resolver resolver(executor);
    tcp::socket socket(executor);

    auto const results = co_await resolver.async_resolve("localhost", "8080", use_awaitable);
    
    co_await asio::async_connect(socket, results, use_awaitable);

    std::string request =
        "GET /hello1 HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Connection: close\r\n"
        "\r\n";

    co_await asio::async_write(socket, asio::buffer(request), use_awaitable);

    std::size_t n = 0;
    char buf[1024];
    while((n = co_await socket.async_read_some(asio::buffer(buf), use_awaitable)) > 0){
        std::cout.write(buf, n);
    }
    std::cout << "\n";
}

void learn02(){
    asio::io_context io_context;
    co_spawn(io_context, async_get_request(), detached);
    io_context.run();
    std::cout << "This is learn02 function." << std::endl;
}