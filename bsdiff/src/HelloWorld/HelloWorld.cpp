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

// ③ 辅助：读整个文件到内存
static std::vector<uint8_t> load_file(const char* path) {
    FILE* f = fopen(path, "rb"); // read binary
    if (!f) throw std::runtime_error(std::string("cannot open: ") + path);
    fseek(f, 0, SEEK_END); // 从文件尾部走0字节
    auto sz = ftell(f); // 返回当前f指针位置，即文件大小
    if (sz < 0) {
        fclose(f);
        throw std::runtime_error("ftell failed");
    }
    rewind(f); // 将指针指向开头
    std::vector<uint8_t> buf(sz);
    fread(buf.data(), 1, sz, f); // 读取到buf中，1字节为单位，读取sz个字节
    fclose(f);
    return buf; // move语义返回，避免不必要的复制
}

// ④ 生成补丁：old_file + new_file → patch_file
//    同时把 newsize 写入补丁头，供还原时使用
// void create_patch(const char* old_file, const char* new_file, const char* patch_file);
void create_patch(const char* old_file, const char* new_file, const char* patch_file) {
    std::vector<uint8_t> old_data = load_file(old_file);
    std::vector<uint8_t> new_data = load_file(new_file);

    FILE* pf = fopen(patch_file, "wb");
    if (!pf) throw std::runtime_error("cannot create patch file");

    // 先写 newsize 到文件头（8 字节），bspatch 时需要知道目标大小
    int64_t newsize = static_cast<int64_t>(new_data.size());
    fwrite(&newsize, sizeof(newsize), 1, pf);

    bsdiff_stream stream{};
    stream.opaque = pf;
    stream.malloc = ::malloc;
    stream.free   = ::free;
    stream.write  = cb_write;

    int ret = bsdiff(old_data.data(), static_cast<int64_t>(old_data.size()),
                     new_data.data(), newsize, &stream);
    fclose(pf);
    if (ret != 0) throw std::runtime_error("bsdiff() failed");
}

// ⑤ 应用补丁：old_file + patch_file → restored_file
// void apply_patch(const char* old_file, const char* new_file, const char* patch_file);
void apply_patch(const char* old_file, const char* new_file, const char* patch_path) {
    std::vector<uint8_t> old_data = load_file(old_file);

    FILE* pf = fopen(patch_path, "rb");
    if (!pf) throw std::runtime_error("cannot open patch file1");

    // 读出之前写入的 newsize
    int64_t newsize = 0;
    fread(&newsize, sizeof(newsize), 1, pf);

    std::vector<uint8_t> new_data(static_cast<size_t>(newsize));

    bspatch_stream stream{};
    stream.opaque = pf;
    stream.read   = cb_read;

    int ret = bspatch(old_data.data(), static_cast<int64_t>(old_data.size()),
                      new_data.data(), newsize, &stream);
    fclose(pf);
    if (ret != 0) throw std::runtime_error("bspatch() failed");

    FILE* out = fopen(new_file, "wb");
    if (!out) throw std::runtime_error("cannot write output");
    fwrite(new_data.data(), 1, static_cast<size_t>(newsize), out);
    fclose(out);
}