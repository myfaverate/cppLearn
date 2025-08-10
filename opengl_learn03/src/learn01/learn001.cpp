#include "learn001.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void helloWorld(){
    std::cout << "Hello World\n";
     // 初始化 GLFW
    int result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // 加上下面 必须 使用 VBO VAO 暂时不加
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 设置 OpenGL 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // 设置 OpenGL 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
#endif

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


    // 设置视口大小
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        std::cout << "Framebuffer size change width: " << width << ", height: " << height << std::endl;
        glViewport(0, 0, width, height);
    });

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 
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
    // 共用一个点
    const float vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left 
        0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    // 设置顶点属性指针
    GLuint location = 0;
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(location);
 
    // 清理状态
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
 
    glClearColor(1.0F, 1.0F, 1.0F, 1.0F); // 设置清屏颜色为白色
 
    // 主循环
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); // 绘制三角形
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void helloWorld2(){

   std::cout << "Hello World2\n";

     // 初始化 GLFW
    int result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // 加上下面 必须 使用 VBO VAO 暂时不加
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 设置 OpenGL 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // 设置 OpenGL 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
#endif

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


    // 设置视口大小
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        std::cout << "Framebuffer size change width: " << width << ", height: " << height << std::endl;
        glViewport(0, 0, width, height);
    });

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
 
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec4 aPosition;
        void main() {
            gl_Position = aPosition;
        }
    )";
 
    const char* fragmentShaderSource1 = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0F, 0.5F, 0.2F, 1.0F); // 红色
        }
    )";
     
    const char* fragmentShaderSource2 = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0F, 1.0F, 0.0F, 1.0F); // 红色
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
 
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, nullptr);
    glCompileShader(fragmentShader1);
    
    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader1, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(fragmentShader1);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

     
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, nullptr);
    glCompileShader(fragmentShader2);
    
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader2, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(fragmentShader2);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }
 
    GLuint shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);
 
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram1, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glDeleteProgram(shaderProgram1);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader1);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    GLuint shaderProgram2 = glCreateProgram();

    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
 
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram2, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glDeleteProgram(shaderProgram2);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader2);
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
 
    // 设置顶点数据
    // 共用一个点
    const float vertices1[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left 
        0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    };


    const float vertices2[] = {
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    };

    GLuint VAOs[2];
    glGenVertexArrays(2, VAOs);

    GLuint VBOs[2];
    glGenBuffers(2, VBOs);

    // ====== 
    
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // 设置顶点属性指针
    GLuint location = 0;
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(location);
    // ======

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(location);
 
    // 清理状态
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
 
    glClearColor(1.0F, 1.0F, 1.0F, 1.0F); // 设置清屏颜色为白色
 
    // 主循环
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // 绘制三角形
        
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // 绘制三角形
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void helloWorld3(){

   std::cout << "Hello World3\n";

     // 初始化 GLFW
    int result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // 加上下面 必须 使用 VBO VAO 暂时不加
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 设置 OpenGL 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // 设置 OpenGL 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
#endif

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


    // 设置视口大小
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        std::cout << "Framebuffer size change width: " << width << ", height: " << height << std::endl;
        glViewport(0, 0, width, height);
    });

    std::ifstream file("../public/hello.txt");
    if(file.is_open()){
        std::string line;
        while(std::getline(file, line)){
            std::cout << line << "\n";
        }
        file.close();
    } else {
        std::cerr << "文件不存在" << "\n";
    }

    int imageWidth, imageHeight, imageChannels;
    auto imageData = stbi_load("../public/container.jpg", &imageWidth, &imageHeight, &imageChannels, STBI_rgb_alpha);
    if(imageData == nullptr){
        throw std::runtime_error("图片加载失败");
    }
    std::cout << "imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", imageChannels: " << imageChannels << "\n";
    std::cout << "Hello World3" << "\n";


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        // layout (location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        // out vec2 TexCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            ourColor = aColor;
            // TexCoord = aTexCoord;
        }
    )";
 
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 ourColor;
        // in vec2 TexCoord;

        // uniform sampler2D ourTexture;

        void main()
        {
            // FragColor = texture(ourTexture, TexCoord);
            FragColor = vec4(ourColor, 1.0);
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

    // GLuint texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // // 为当前绑定的纹理对象设置环绕、过滤方式
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    // glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData); // 用完就释放

    // const float vertices[] = {
    //     //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    //     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    // };

    const float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
    };

    const GLuint indices[] = {
        0, 1, 2, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };



    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);

    GLuint EBO;
    glGenBuffers(1, &EBO);

    // ====== 
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    GLuint location = 0;
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(location);
    
    GLuint location1 = 1;
    glVertexAttribPointer(location1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(location1);

    // 清理状态
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // glBindTexture(GL_TEXTURE_2D, 0);
 
    glClearColor(1.0F, 1.0F, 1.0F, 1.0F); // 设置清屏颜色为白色

        // 主循环
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 如果解绑过，这里要重新绑定
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制三角形
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void helloWorld4(){

   std::cout << "Hello World3\n";

     // 初始化 GLFW
    int result = glfwInit();
    if (result == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // 加上下面 必须 使用 VBO VAO 暂时不加
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 设置 OpenGL 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // 设置 OpenGL 次版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
#endif

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


    // 设置视口大小
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) {
        std::cout << "Framebuffer size change width: " << width << ", height: " << height << std::endl;
        glViewport(0, 0, width, height);
    });

    std::ifstream file("../public/hello.txt");
    if(file.is_open()){
        std::string line;
        while(std::getline(file, line)){
            std::cout << line << "\n";
        }
        file.close();
    } else {
        std::cerr << "文件不存在" << "\n";
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec3 ourColor;
        out vec2 TexCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            ourColor = aColor;
            TexCoord = aTexCoord;
        }
    )";
 
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 ourColor;
        in vec2 TexCoord;

        // uniform sampler2D ourTexture;
        uniform sampler2D texture1;
        uniform sampler2D texture2;

        void main()
        {
            // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
            FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
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

    const float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    const GLuint indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };



    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);

    GLuint EBO;
    glGenBuffers(1, &EBO);

    // ====== 
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    GLuint location = 0;
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    glEnableVertexAttribArray(location);
    
    GLuint location1 = 1;
    glVertexAttribPointer(location1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(location1);

    GLuint location2 = 2;
    glVertexAttribPointer(location2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(location2);

    // 图片1
    stbi_set_flip_vertically_on_load(true);
    int imageWidth, imageHeight, imageChannels;
    auto imageData = stbi_load("../public/container.jpg", &imageWidth, &imageHeight, &imageChannels, STBI_rgb_alpha);
    if(imageData == nullptr){
        throw std::runtime_error("图片加载失败");
    }
    std::cout << "imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", imageChannels: " << imageChannels << "\n";
    std::cout << "Hello World3" << "\n";

    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData); // 用完就释放

    // 图片2
    imageData = stbi_load("../public/awesomeface.png", &imageWidth, &imageHeight, &imageChannels, STBI_rgb_alpha);
    if(imageData == nullptr){
        throw std::runtime_error("图片加载失败");
    }
    std::cout << "imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", imageChannels: " << imageChannels << "\n";
    std::cout << "Hello World3" << "\n";

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imageData); // 用完就释放

    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);

    // 清理状态
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
 
    glClearColor(1.0F, 1.0F, 1.0F, 1.0F); // 设置清屏颜色为白色

        // 主循环
    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 如果解绑过，这里要重新绑定
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制三角形
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}