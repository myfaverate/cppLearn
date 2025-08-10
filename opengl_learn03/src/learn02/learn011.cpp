#include "learn011.hpp"

void learn02HelloWorld(){
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
    auto imageData = stbi_load("../public/image15.jpg", &imageWidth, &imageHeight, &imageChannels, STBI_rgb_alpha);
    std::cout << "learn02HelloWorld imageWidth: " << imageWidth << ", imageHeight: " << imageHeight << ", imageChannels: " << imageChannels << "\n";
    stbi_image_free(imageData); // 用完就释放
    std::cout << "learn02HelloWorld Hello World3" << "\n";
}

void learn02HelloWorld1(){
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
    // 下面这行代码就需要改为:
    // glm::mat4 trans = glm::mat4(1.0f)
    // 之后将不再进行提示
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;
}