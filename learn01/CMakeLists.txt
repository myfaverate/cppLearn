cmake_minimum_required(VERSION 3.10.0)
project(learn01 VERSION 0.1.0 LANGUAGES C CXX)

# ✅ 设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)  # 使用 C++26 标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（使用纯标准）


# 如果是单配置生成器（Makefile/Ninja)，在未指定时默认使用 Release
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type specified, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# 查找源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.c"
) 

add_executable(learn01 main.cpp ${SOURCES})

# 设置头文件包含路径 
target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/learn01
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/learn02
)

# —— 为目标按配置设置编译选项 —— 
# Release: -O2 (GCC/Clang) 或 /O2 (MSVC)，并加上 -DNDEBUG
# Debug: 指定更合适的 Debug 标志
if (MSVC)
    message(STATUS "Using MSVC compiler settings")
    target_compile_options(learn01 PRIVATE
        $<$<CONFIG:Release>:/O3 /DNDEBUG>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
else()
    message(STATUS "Using GCC/Clang compiler settings")
    target_compile_options(learn01 PRIVATE
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
        $<$<CONFIG:Debug>:-Og -g>
    )
endif()

# 可选：对 Release 开启跨模块优化（LTO），如果编译器/链接器支持
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

# 5. 链接 Boost 库
find_package(Boost CONFIG REQUIRED COMPONENTS system) 

# 链接 Boost 库
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Boost::system)
# Windows 需要额外链接 Winsock2、Mswsock
if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ws2_32 Mswsock)
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
