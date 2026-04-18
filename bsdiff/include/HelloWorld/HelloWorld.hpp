#ifndef HELLOWORLD_HPP
#define HELLOWORLD_HPP

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <string>

// ① 不能直接 include 头文件（'new' 是 C++ 关键字），改为手动前向声明
extern "C" {
    struct bsdiff_stream {
        void* opaque;
        void* (*malloc)(size_t size);
        void  (*free)(void* ptr);
        int   (*write)(struct bsdiff_stream* stream, const void* buffer, int size);
    };

    struct bspatch_stream {
        void* opaque;
        int   (*read)(const struct bspatch_stream* stream, void* buffer, int length);
    };

    // 参数名从 'new' 改为 'new_data'，避免关键字冲突
    int bsdiff(const uint8_t* old_data, int64_t oldsize,
               const uint8_t* new_data, int64_t newsize,
               struct bsdiff_stream* stream);

    int bspatch(const uint8_t* old_data, int64_t oldsize,
                uint8_t* new_data, int64_t newsize,
                struct bspatch_stream* stream);
}

void create_patch(const char* old_file, const char* new_file, const char* patch_file);
void apply_patch(const char* old_file, const char* new_file, const char* patch_file);

#endif // HELLOWORLD_HPP