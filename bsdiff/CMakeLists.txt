cmake_minimum_required(VERSION 3.10.0)
project(bsdiff VERSION 0.1.0 LANGUAGES C CXX)

# ✅ 设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（使用纯标准）


# 查找源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.c"
)

message("CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")

# add_library(${CMAKE_PROJECT_NAME} SHARED main.cpp ${SOURCES})
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${SOURCES})

if(WIN32)
# 让 JNI 在 Windows/MinGW 下生成 hello.dll，避免默认的 libhello.dll
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME ${CMAKE_PROJECT_NAME}
)
endif()

# 设置头文件包含路径
target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/bsdiff
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/HelloWorld
)


if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX)
else()
    message("no MSVC")
    target_compile_options(
        ${CMAKE_PROJECT_NAME} PRIVATE -Wall
        -Werror # 警告转错误
        -Wextra # 发现更多潜在的逻辑和风格问题
        -pedantic # 追求极致的标准兼容性
        -D NDEBUG # 定义 NDEBUG 以禁用 assert() 的评估
    )
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
