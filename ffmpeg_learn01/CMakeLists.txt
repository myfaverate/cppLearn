cmake_minimum_required(VERSION 3.10.0)
project(ffmpeg_learn01 VERSION 0.1.0 LANGUAGES C CXX)

# ✅ 设置 C++ 标准
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（使用纯标准）

message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

set(FFMPEG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/ffmpeg")
set(FFMPEG_LIB_DIR "${CMAKE_SOURCE_DIR}/libs/ffmpeg")
set(FFMPEG_BIN_DIR "${CMAKE_SOURCE_DIR}/bins/ffmpeg")
message("FFMPEG_INCLUDE_DIR: ${FFMPEG_INCLUDE_DIR}")
message("FFMPEG_LIB_DIR: ${FFMPEG_LIB_DIR}")
message("FFMPEG_BIN_DIR: ${FFMPEG_BIN_DIR}")

# 2. 查找 FFmpeg 库
find_library(AVCODEC_LIB avcodec PATHS ${FFMPEG_LIB_DIR} REQUIRED)
find_library(AVFORMAT_LIB avformat PATHS ${FFMPEG_LIB_DIR} REQUIRED)
find_library(AVUTIL_LIB avutil PATHS ${FFMPEG_LIB_DIR} REQUIRED)
find_library(SWRESAMPLE_LIB swresample PATHS ${FFMPEG_LIB_DIR} REQUIRED)
find_library(SWSCALE_LIB swscale PATHS ${FFMPEG_LIB_DIR} REQUIRED)

# 查找源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "src/*.cpp"
)

add_executable(ffmpeg_learn01 main.cpp ${SOURCES})

# 设置头文件包含路径
target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/learn
    PUBLIC ${FFMPEG_INCLUDE_DIR} # ffmpeg
)
message("PROJECT_NAME: ${PROJECT_NAME}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
# 5. 链接 FFmpeg 库
target_link_libraries(${PROJECT_NAME}
    ${AVCODEC_LIB}
    ${AVFORMAT_LIB}
    ${AVUTIL_LIB}
    ${SWRESAMPLE_LIB}
    ${SWSCALE_LIB}
)

# 6. 复制 DLL 到输出目录（Windows 必需）
# if(WIN32)
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy
#             "${FFMPEG_BIN_DIR}/avcodec-61.dll"
#             "${FFMPEG_BIN_DIR}/avformat-61.dll"
#             "${FFMPEG_BIN_DIR}/avutil-59.dll"
#             "${FFMPEG_BIN_DIR}/swresample-5.dll"
#             "${FFMPEG_BIN_DIR}/swscale-8.dll"
#             $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     )
# endif()

# 运行时自动复制 DLL（使用 bin/ 下的文件）
# if(WIN32)
#     file(GLOB FFMPEG_DLLS "${FFMPEG_BIN_DIR}/*.dll")
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy
#             ${FFMPEG_DLLS}
#             $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     )
# endif()

if(WIN32)
    message("FFMPEG_BIN_DIR1: ${FFMPEG_BIN_DIR}")
    file(GLOB FFMPEG_DLLS "${FFMPEG_BIN_DIR}/*.dll")
    message("DLLs to copy: ${FFMPEG_DLLS}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${FFMPEG_DLLS}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
