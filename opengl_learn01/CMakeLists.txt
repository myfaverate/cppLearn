cmake_minimum_required(VERSION 3.10.0)
project(opengl_learn01 VERSION 0.1.0 LANGUAGES C CXX)

# ✅ 设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（使用纯标准）

# 查找源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.c"
)

# 查找包
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)
# ###########

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${SOURCES})


# 设置头文件包含路径
target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/learn01
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/std
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 查找包
find_package(OpenGL REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenGL::GL glfw)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
