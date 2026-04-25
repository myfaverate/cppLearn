#ifndef BSDIFF_JNI_HPP
#define BSDIFF_JNI_HPP

#include "HelloWorld.hpp"

extern "C" {
    /**
 * @author: zsh
 * @old_file: 旧版本文件路径
 * @new_file: 新版本文件路径
 * @patch_file: 生成的补丁文件路径
 * @return: 0 成功，非0 失败
 */
int bsdiff_create_patch(const char* old_file, const char* new_file,
                  const char* patch_file);
/**
 * @old_file: 旧版本文件路径
 * @new_file: 还原后的新版本文件路径
 * @patch_file: 补丁文件路径
 * @return: 0 成功，非0 失败
 */
int bsdiff_apply_patch(const char* old_file, const char* new_file,
                 const char* patch_file);
}

#endif // BSDIFF_JNI_HPP