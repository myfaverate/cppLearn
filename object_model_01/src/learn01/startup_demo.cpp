#include <cstdlib>
#include <iostream>

// ---------------------------------------------------------------------------
// 1. __attribute__((constructor/destructor)) — 编译器注入 .init_array / .fini_array
//    这些函数分别在 main 之前和 main 之后被调用
// ---------------------------------------------------------------------------

__attribute__((constructor)) auto before_main_prio_101() -> void {
    std::cout << "[.init_array] constructor attribute (priority 101)" << '\n';
}

__attribute__((constructor)) auto before_main_default() -> void {
    std::cout << "[.init_array] constructor attribute (default)" << '\n';
}

__attribute__((destructor)) auto after_main_default() -> void {
    std::cout << "[.fini_array] destructor attribute (default)" << '\n';
}

__attribute__((destructor)) auto after_main_prio_101() -> void {
    std::cout << "[.fini_array] destructor attribute (priority 101)" << '\n';
}

// ---------------------------------------------------------------------------
// 2. 全局对象 — 构造函数在 main 之前运行，析构函数在 main 之后运行
//    同一翻译单元内按定义顺序构造，按逆序析构
// ---------------------------------------------------------------------------

struct GlobalObj {
    const char* name;

    explicit GlobalObj(const char* n) : name(n) {
        std::cout << "[Global ctor] " << name << '\n';
    }

    ~GlobalObj() {
        std::cout << "[Global dtor] " << name << '\n';
    }

    GlobalObj(const GlobalObj&) = delete;
    auto operator=(const GlobalObj&) -> GlobalObj& = delete;
};

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
inline GlobalObj g_obj1{"g_obj1 (global)"};
inline GlobalObj g_obj2{"g_obj2 (global)"};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

// ---------------------------------------------------------------------------
// 3. 演示函数 — 在 main 中调用，展示 atexit 和 function-local static 的析构时机
// ---------------------------------------------------------------------------

namespace startup_demo {

auto run() -> void {
    std::cout << "[main] executing..." << '\n';

    // atexit 注册的回调：在 main 返回后、全局析构之前执行（LIFO）
    std::atexit([] {
        std::cout << "[atexit #1] first registered, last called" << '\n';
    });
    std::atexit([] {
        std::cout << "[atexit #2] second registered, first called" << '\n';
    });

    // function-local static 对象：第一次执行到这里时构造；
    // 程序结束时（全局析构阶段之前）析构
    static GlobalObj static_local{"static_local_obj"};
    (void)static_local;

    std::cout << "[main] leaving..." << '\n';
}

} // namespace startup_demo
