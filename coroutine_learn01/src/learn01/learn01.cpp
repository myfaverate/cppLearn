#include "learn01.hpp"

// 一个简单的协程返回类型
struct HelloTask {
    struct promise_type {
        HelloTask get_return_object() {
            return HelloTask{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() { return {}; }  // 直接开始执行
        std::suspend_never final_suspend() noexcept {
            return {};
        }  // 结束后不挂起
        void return_void() {}
        void unhandled_exception() { std::exit(1); }
    };

    std::coroutine_handle<promise_type> handle;

    HelloTask(std::coroutine_handle<promise_type> h) : handle(h) {}
    ~HelloTask() {
        if (handle) handle.destroy();
    }
};

// 协程函数
HelloTask hello() {
    std::cout << "Hello from coroutine!" << std::endl;
    co_return;
}

int64_t currentTimeMillis() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration)
        .count();
}

/**
 * https://zplutor.github.io/2022/03/25/cpp-coroutine-beginner/
 * https://www.bennyhuo.com/book/cpp-coroutines/00-foreword.html
 */
void message() {
    std::jthread thread([]() {
        std::cout << "Hello from thread! time1: " << currentTimeMillis()
                  << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::this_thread::get_id();
        std::cout << "Hello from thread! time2: " << currentTimeMillis()
                  << std::endl;
    });
    std::future<int> result1 = std::async(std::launch::async, []() -> int {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Hello from async! time1: " << currentTimeMillis()
                  << std::endl;
        return 2;
    });
    std::future<int> result2 = std::async(std::launch::async, []() -> int {
        std::cout << "Hello from async! time2: " << currentTimeMillis()
                  << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 3;
    });
    auto result = result1.get() + result2.get();
    std::cout << "Result from async: " << result << std::endl;
    std::cout << "add..\n";
}

// 窗口过程函数
// LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM
// lParam)
// {
//     switch (uMsg)
//     {
//         case WM_PAINT:
//         {
//             PAINTSTRUCT ps;
//             HDC hdc = BeginPaint(hwnd, &ps);

//             // 在窗口中央绘制"Hello, World!"
//             RECT rect;
//             GetClientRect(hwnd, &rect);
//             DrawTextW(hdc, L"Hello, World!", -1, &rect,
//                     DT_SINGLELINE | DT_CENTER | DT_VCENTER);

//             EndPaint(hwnd, &ps);
//             return 0;
//         }
//         case WM_DESTROY:
//             PostQuitMessage(0);
//             return 0;
//     }
//     return DefWindowProcW(hwnd, uMsg, wParam, lParam);
// }

// 程序入口点
// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR
// pCmdLine, int nCmdShow)
// {
//     // 注册窗口类
//     const wchar_t CLASS_NAME[] = L"HelloWorldWindowClass";

//     WNDCLASSW wc = {};
//     wc.lpfnWndProc = WindowProc;
//     wc.hInstance = hInstance;
//     wc.lpszClassName = CLASS_NAME;

//     RegisterClassW(&wc);

//     // 创建窗口
//     HWND hwnd = CreateWindowExW(
//         0,                              // 扩展窗口样式
//         CLASS_NAME,                     // 窗口类
//         L"Hello, World!",               // 窗口标题
//         WS_OVERLAPPEDWINDOW,           // 窗口样式

//         // 位置和大小
//         CW_USEDEFAULT, CW_USEDEFAULT,
//         400, 300,

//         NULL,       // 父窗口
//         NULL,       // 菜单
//         hInstance,  // 实例句柄
//         NULL        // 额外应用数据
//     );

//     if (hwnd == NULL)
//     {
//         return 0;
//     }

//     ShowWindow(hwnd, nCmdShow);

//     // 消息循环
//     MSG msg = {};
//     while (GetMessage(&msg, NULL, 0, 0))
//     {
//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

//     return 0;
// }

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_LBUTTONDOWN: {
            // 处理鼠标左键点击事件
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hwnd, &pt);
            std::wcout << L"Mouse clicked at: (" << pt.x << L", " << pt.y
                       << L")" << std::endl;
            auto result = MessageBoxW(hwnd, L"Mouse clicked!", L"好的单子", MB_OKCANCEL);
            if (result == IDOK) {
                std::wcout << L"User clicked OK." << std::endl;
            } else {
                std::wcout << L"User clicked Cancel." << std::endl;
            }
            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            DrawTextW(hdc, L"Hello, World!", -1, &rect,
                      DT_SINGLELINE | DT_CENTER | DT_VCENTER);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

// 封装窗口创建和消息循环
void CreateHelloWorldWindow() {

    HINSTANCE hInstance = GetModuleHandleW(nullptr);

    // 注册窗口类
    const std::wstring_view CLASS_NAME = L"HelloWorldWindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME.data();

    RegisterClassW(&wc);

    // 创建窗口
    HWND hwnd = CreateWindowExW(0,                    // 扩展窗口样式
                                CLASS_NAME.data(),    // 窗口类
                                L"Hello, World!",     // 窗口标题
                                WS_OVERLAPPEDWINDOW,  // 窗口样式

                                // 位置和大小
                                CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,

                                nullptr,    // 父窗口
                                nullptr,    // 菜单
                                hInstance,  // 实例句柄
                                nullptr     // 额外应用数据
    );

    if (hwnd == nullptr) {
        return;
    }

    ShowWindow(hwnd, SW_SHOW);

    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// 处理点击的函数
void handlerClick() {
    printf("Starting GUI window...\n");
    CreateHelloWorldWindow();  // 调用窗口创建函数
    printf("Window closed.\n");
}