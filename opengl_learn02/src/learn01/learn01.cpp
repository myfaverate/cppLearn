#include "learn01.hpp"

void learn01() {
    std::cout << "Learning C++ is fun!" << std::endl;
}
void simpleOpenGL(){

     // 初始化 GLFW
    int result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }
    
    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Hello World", nullptr, nullptr);
    if(window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window); // 设置当前上下文
    
    result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (result == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec4 aPosition;
        void main() {
            gl_Position = aPosition;
            gl_PointSize = 50.0;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // 红色
        }
    )";

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(fragmentShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }
    
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 设置顶点数据
    const float vertices[] = {
        0.8f,  -0.8f, 0.0f, // 右下
       -0.8f,  -0.8f, 0.0f, // 左下
        0.0f,   0.8f, 0.0f  // 上
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 设置顶点属性指针
    GLuint location = 0;
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(location);

    // 清理状态
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(1.0F, 1.0F, 1.0F, 1.0F); // 设置清屏颜色为白色
    // 启用点大小
    glEnable(GL_PROGRAM_POINT_SIZE);

    // 主循环
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 3); // 绘制三角形
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void simpleOpenGL1(){

    // 初始化 GLFW
    int result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }
    
    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Hello World", nullptr, nullptr);
    if(window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window); // 设置当前上下文

    result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (result == 0) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec4 aPosition;
        void main() {
            gl_Position = aPosition;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0); // 红色
        }
    )";

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(fragmentShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    GLint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 设置顶点数据
    const float vertices[] = {
        0.8f,  -0.8f, 0.0f, // 右下
       -0.8f,  -0.8f, 0.0f, // 左下
        0.0f,   0.8f, 0.0f  // 上
    };

    GLuint location = 0;
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), vertices);
    glClearColor(1.0F, 1.0F, 1.0F, 1.0f);
    
    // 主循环
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}