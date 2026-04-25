#ifndef HELLOWORLD_HPP
#define HELLOWORLD_HPP

extern "C" {
#include "bsdiff.h"
#include "bspatch.h"
}

#include <fcntl.h>  // open
#include <sys/mman.h> // mmap, munmap, MAP_FAILED
#include <sys/stat.h> // stat
#include <unistd.h> // close, fstat, ftruncate

#include <cstdio>
#include <stdexcept>

/**
 * @author: zsh
 * @old_file: 旧版本文件路径
 * @new_file: 新版本文件路径
 * @patch_file: 生成的补丁文件路径
 */
void create_patch(const char* old_file, const char* new_file,
                  const char* patch_file);
/**
 * @old_file: 旧版本文件路径
 * @new_file: 还原后的新版本文件路径
 * @patch_file: 补丁文件路径
 */
void apply_patch(const char* old_file, const char* new_file,
                 const char* patch_file);

// ─── RAII helpers ────────────────────────────────────────────────
struct FdGuard {
    const int fd;
    explicit FdGuard(const int fd) : fd(fd) {}
    ~FdGuard() {
        if (fd >= 0) close(fd);
    }
    FdGuard(const FdGuard&) = delete;
};

struct MmapGuard {
    void* addr;
    const size_t size;
    MmapGuard(void* addr, const size_t size) : addr(addr), size(size) {}
    ~MmapGuard() {
        if (addr != MAP_FAILED && size > 0) munmap(addr, size);
    }
    MmapGuard(const MmapGuard&) = delete;
};

struct FileGuard {
    FILE* f;
    explicit FileGuard(FILE* f) : f(f) {}
    ~FileGuard() {
        if (f) fclose(f);
    }
    FileGuard(const FileGuard&) = delete;
};

#endif  // HELLOWORLD_HPP