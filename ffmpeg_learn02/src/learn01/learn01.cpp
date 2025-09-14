#include "learn01.hpp"

void learn01(){
    std::cout << "Hello Learn01\n";
    // const auto javaHome = std::getenv("JAVA_HOME");
    // const auto path = std::getenv("PATH");
    // std::cout << "javaHome: " << javaHome << "\n";
    // std::cout << "path: " << path << "\n";
    // const auto currentPath = std::filesystem::current_path().generic_string();
    // std::cout << "currentPath: " << currentPath << "\n";

    // ====
    // 打印 FFmpeg 各个库的版本
    std::cout << "libavcodec version: " 
              << AV_VERSION_MAJOR(avcodec_version()) << "."
              << AV_VERSION_MINOR(avcodec_version()) << "."
              << AV_VERSION_MICRO(avcodec_version()) << std::endl;
    // avcodec_register_all(); // 对新版本 FFmpeg 可以忽略
std::cout << "libavcodec version: " << AV_VERSION_MAJOR(avcodec_version()) << std::endl;
// std::cout << "libavformat version: " << avformat_version() << std::endl;
// std::cout << "libavutil   version: " << avutil_version()   << std::endl;
std::cout << "Build configuration:" << std::endl;
std::cout << avcodec_configuration() << std::endl;
    // std::cout << "libavformat version: " << avformat_version() << std::endl;
    // std::cout << "libavutil   version: " << avutil_version()   << std::endl;

    // // 打印 FFmpeg 构建时的配置参数（包含启用的第三方库信息）
    // std::cout << "Build configuration:" << std::endl;
    // std::cout << avcodec_configuration() << std::endl;
}