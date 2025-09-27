#include "learn01.hpp"

namespace learn01 {

void learn01() {
    // Function implementation goes here
    std::cout << "Learning C++17 features!" << std::endl;
    // 初始化 GLFW
    int result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }
    // 创建窗口
    GLFWwindow* window =
        glfwCreateWindow(640, 480, "OpenGL Hello World", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);  // 设置当前上下文
    result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (result == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }
    // 主循环
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        // 设置背景色为红色（类似 Hello World 效果）
        glClearColor(1.0F, 0.0F, 0.0F, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

struct Vector {
    float x, y, z;
    auto operator<=>(const Vector&) const = default; // C++20 三向比较运算符
};

void learn02() {
    std::cout << "Learning C++20 features!" << std::endl;
    // Function implementation goes here
    
}

}  // namespace learn01