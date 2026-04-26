cmake_minimum_required(VERSION 3.10.0)
project(object_model_01 VERSION 0.1.0 LANGUAGES C CXX)

# ✅ 设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)  # 使用 C++26 标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（使用纯标准）

# 查找源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.c"
) 

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${SOURCES})

# 设置头文件包含路径 
target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/learn01
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/learn02
)

# 1. 寻找 OpenSSL 软件包
find_package(OpenSSL REQUIRED)   

# 3. 链接 OpenSSL 库
# OpenSSL::Crypto 对应你命令行里的 -lcrypto
# 如果你需要用到 SSL 相关功能（如 https），可以再加上 OpenSSL::SSL
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenSSL::Crypto)

# 可选：打印找到的路径，方便调试
message(STATUS "OpenSSL Include: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL Libraries: ${OPENSSL_LIBRARIES}")


if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX)
else()
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

# --- Test executable ---
add_executable(test_sha256sum
    tests/test_sha256sum.cpp
    src/learn02/sha256sum.cpp
)
target_include_directories(test_sha256sum
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/learn02
)
target_link_libraries(test_sha256sum PRIVATE OpenSSL::Crypto)
if(MSVC)
    target_compile_options(test_sha256sum PRIVATE /W4 /WX)
else()
    target_compile_options(test_sha256sum PRIVATE -Wall -Wextra -pedantic)
endif()

add_test(NAME sha256sum_test COMMAND test_sha256sum)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
