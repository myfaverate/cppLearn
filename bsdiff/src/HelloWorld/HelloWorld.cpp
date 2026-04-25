#include "HelloWorld.hpp"

// ② 回调实现 —— 以 FILE* 作为 opaque 上下文
static int cb_write(struct bsdiff_stream* stream, const void* buffer, int size) {
    FILE* f = static_cast<FILE*>(stream->opaque);
    return (fwrite(buffer, 1, size, f) == static_cast<size_t>(size)) ? 0 : -1;
}

static int cb_read(const struct bspatch_stream* stream, void* buffer, int length) {
    FILE* f = static_cast<FILE*>(stream->opaque);
    return (fread(buffer, 1, length, f) == static_cast<size_t>(length)) ? 0 : -1;
}

/**
 * ④ 生成补丁：old_file + new_file → patch_file, 同时把 newsize 写入补丁头，供还原时使用
 */
void create_patch(const char* old_file, const char* new_file, const char* patch_file) {
    // mmap 方式读取文件，避免一次性加载到内存中（尤其是大文件）
    // 旧文件
    const FdGuard old_fd_guard(open(old_file, O_RDONLY));
    if (old_fd_guard.fd < 0) throw std::runtime_error("cannot open old file");
    struct stat old_st;
    if (fstat(old_fd_guard.fd, &old_st) < 0) throw std::runtime_error("cannot stat old file");
    const size_t old_size = old_st.st_size;
    
    const MmapGuard old_mmap_guard(old_size > 0 ? mmap(nullptr, old_size, PROT_READ, MAP_PRIVATE, old_fd_guard.fd, 0) : nullptr, old_size);
    if (old_size > 0 && old_mmap_guard.addr == MAP_FAILED) throw std::runtime_error("cannot mmap old file");

    // 更新文件
    const FdGuard new_fd_guard(open(new_file, O_RDONLY));
    if (new_fd_guard.fd < 0) throw std::runtime_error("cannot open new file");
    struct stat new_st;
    if (fstat(new_fd_guard.fd, &new_st) < 0) throw std::runtime_error("cannot stat new file");
    const size_t new_size = new_st.st_size;
    const MmapGuard new_mmap_guard(new_size > 0 ? mmap(nullptr, new_size, PROT_READ, MAP_PRIVATE, new_fd_guard.fd, 0) : nullptr, new_size);
    if (new_size > 0 && new_mmap_guard.addr == MAP_FAILED) throw std::runtime_error("cannot mmap new file");

    // 生成的patch文件
    const FileGuard patch_file_guard(fopen(patch_file, "wb"));
    if (!patch_file_guard.f) throw std::runtime_error("cannot create patch file");

    // 先写 newsize 到文件头（8 字节），bspatch 时需要知道目标大小
    int64_t newsize = static_cast<int64_t>(new_size);
    fwrite(&newsize, sizeof(newsize), 1, patch_file_guard.f);

    bsdiff_stream stream{};
    stream.opaque = patch_file_guard.f;
    stream.malloc = ::malloc;
    stream.free   = ::free;
    stream.write  = cb_write;

    const int ret = bsdiff(static_cast<const uint8_t*>(old_mmap_guard.addr), static_cast<int64_t>(old_size),
                     static_cast<const uint8_t*>(new_mmap_guard.addr), newsize, &stream);

    if (ret != 0) throw std::runtime_error("bsdiff() failed");
}
/**
 * ⑤ 应用补丁：old_file + patch_file → restored_file 
 */
void apply_patch(const char* old_file, const char* new_file, const char* patch_path) {
    // 旧文件
    const FdGuard old_fd_guard(open(old_file, O_RDONLY));
    if (old_fd_guard.fd < 0) throw std::runtime_error("cannot open old file");
    struct stat old_st;
    if (fstat(old_fd_guard.fd, &old_st) < 0) throw std::runtime_error("cannot stat old file");
    const size_t old_size = old_st.st_size;
    const MmapGuard old_mmap_guard(old_size > 0 ? mmap(nullptr, old_size, PROT_READ, MAP_PRIVATE, old_fd_guard.fd, 0) : nullptr, old_size);
    if (old_size > 0 && old_mmap_guard.addr == MAP_FAILED) throw std::runtime_error("cannot mmap old file");

    // patch 文件
    const FileGuard patch_file_guard(fopen(patch_path, "rb"));
    if (!patch_file_guard.f) throw std::runtime_error("cannot open patch file");

    // 读出之前写入的 newsize
    int64_t new_size = 0;
    //                  8字节             读取一个
    if(fread(&new_size, sizeof(new_size), 1, patch_file_guard.f) != 1) {
        throw std::runtime_error("cannot read new size from patch file");
    }

    // 新文件
    const FdGuard new_fd_guard(open(new_file, O_RDWR | O_CREAT | O_TRUNC, 0644)); // 注意：需要先创建并设置大小，否则 mmap 时会失败 / rw-r--r-- 0644 权限
    if (new_fd_guard.fd < 0) throw std::runtime_error("cannot create new file");
    if (ftruncate(new_fd_guard.fd, new_size) < 0) throw std::runtime_error("cannot set new file size");
    printf("new file size: %lld bytes\n", new_size);
    const MmapGuard new_mmap_guard(new_size > 0 ? mmap(nullptr, new_size, PROT_READ | PROT_WRITE, MAP_SHARED, new_fd_guard.fd, 0) : nullptr, new_size);
    if (new_size > 0 && new_mmap_guard.addr == MAP_FAILED) throw std::runtime_error("cannot mmap new file");

    bspatch_stream stream{};
    stream.opaque = patch_file_guard.f;
    stream.read   = cb_read;

    int ret = bspatch(static_cast<const uint8_t*>(old_mmap_guard.addr), static_cast<int64_t>(old_size),
                      static_cast<uint8_t*>(new_mmap_guard.addr), new_size, &stream);

    if (ret != 0) throw std::runtime_error("bspatch() failed");
}