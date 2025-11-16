cmake_minimum_required(VERSION 3.10.0)
project(mysql_learn01 VERSION 0.1.0 LANGUAGES C CXX)


# ✅ 设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（使用纯标准）

# 查找源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "src/*.c"
)

add_executable(mysql_learn01 main.cpp ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/learn01
    PRIVATE D:/SoftWare/msys64/mysql1/include/jdbc
    PRIVATE D:/SoftWare/msys64/mysql1/include
)

foreach(env $ENV{PATH})
    message("env: ${env}")
endforeach()


target_link_libraries(${CMAKE_PROJECT_NAME} 
    PRIVATE mysqlcppconn  # 传统api
    PRIVATE mysqlcppconnx # session api
)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
