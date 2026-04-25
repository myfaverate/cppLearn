#include "bsdiff_jni.hpp"

extern "C" {
    int bsdiff_create_patch(const char* old_file, const char* new_file,
                            const char* patch_file) {
        // TODO: 实现 bsdiff_create_patch 函数
        try {
            create_patch(old_file, new_file, patch_file);
            return 0; // 成功
        } catch (const std::exception& e) {
            fprintf(stderr, "Error creating patch: %s\n", e.what());
            return -1; // 失败
        }
    }

    int bsdiff_apply_patch(const char* old_file, const char* new_file,
                           const char* patch_file) {
        // TODO: 实现 bsdiff_apply_patch 函数
        try {
            apply_patch(old_file, new_file, patch_file);
            return 0; // 成功
        } catch (const std::exception& e) {
            fprintf(stderr, "Error applying patch: %s\n", e.what());
            return -1; // 失败
        }
    }
}