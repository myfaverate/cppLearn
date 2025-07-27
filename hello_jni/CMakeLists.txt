cmake_minimum_required(VERSION 3.22.1)
project(hello VERSION 0.1.0 LANGUAGES C CXX)

# ✅ 设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（使用纯标准）


# 查找源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "src/*.cpp"
)
message("SOURCES: ${SOURCES}")
message("CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")

# 设置头文件目录
# 设置头文件包含目录
message(CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME})
message(PROJECT_NAME: ${PROJECT_NAME})
message(CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${CMAKE_PROJECT_NAME} SHARED  hello.cpp ${SOURCES})

# 设置头文件包含路径
target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
