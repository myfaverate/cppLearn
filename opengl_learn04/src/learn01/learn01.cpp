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

class Matrix {
public:
    // 行、列
    const int rows, cols;
    // 元素
    float** data;
    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        std::cout << "Matrix constructor...\n";
        try {
            data = new float*[rows];
            for (int i = 0; i < rows; i++) {
                data[i] = new float[cols]();  // 调用float[]的默认构造函数，初始化为0F
            }
        } catch (const std::exception& e) {
            // 捕获内存不足异常、然后回收内存
            for (int i = 0; i < rows; i++) {
                delete[] data[i];  // 先删值
            }
            delete[] data;  // 后删指针
            std::cerr << e.what() << '\n';
            throw; // 继续抛出异常
        }
    }
    ~Matrix() {
        std::cout << "Matrix destructor...\n";
        for(int i = 0; i < rows; i ++ ){
            delete[] data[i]; // 先删值
        }
        delete[] data; // 后删指针
    }
    void printMatrix() {
        for(int i = 0; i < rows; i ++ ){
            for(int j = 0; j < cols; j ++ ){
                std::cout << data[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    // 实现深拷贝
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        std::cout << "Matrix copy constructor...\n";
        data = new float*[rows];
        for(int i = 0; i < rows; i ++ ){
            data[i] = new float[cols];
            for(int j = 0; j < cols; j ++ ){
                data[i][j] = other.data[i][j];
            }
        }
    }
    // 实现移动语义
    Matrix(Matrix&& other) noexcept : rows(other.rows), cols(other.cols), data(other.data) {
        std::cout << "Matrix move constructor...\n";
        other.data = nullptr; // 将源对象的指针置为空，防止析构时重复释放
    }
    // 实现移动赋值
    Matrix& operator=(Matrix&& other) noexcept {
        std::cout << "Matrix move assignment...\n";
        if (this != &other) {
            // 释放当前对象的资源
            for(int i = 0; i < rows; i ++ ){
                delete[] data[i];
            }
            delete[] data;
            // 移动资源
            data = other.data;
            other.data = nullptr; // 将源对象的指针置为空，防止析构时重复释放
        }
        return *this;
    }
};

void learn02() {
    Matrix m(3, 4);
    m.printMatrix();
}

}  // namespace learn01