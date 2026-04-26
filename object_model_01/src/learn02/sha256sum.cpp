#include <openssl/evp.h>

#include <array>
#include <cstdio>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>

#include "sha256sum.hpp"

namespace sha256sum {

auto hash_file(std::string_view filepath) -> std::string {
    using UniqueFile = std::unique_ptr<std::FILE, decltype([](std::FILE* f) {
        if (f) std::fclose(f);
    })>;

    auto path = std::string{filepath};
    UniqueFile file{std::fopen(path.c_str(), "rb")};
    if (!file) {
        throw std::runtime_error(
            std::format("cannot open file: {}", filepath));
    }

    using UniqueDigestCtx = std::unique_ptr<EVP_MD_CTX, decltype([](EVP_MD_CTX* c) {
        EVP_MD_CTX_free(c);
    })>;

    UniqueDigestCtx ctx{EVP_MD_CTX_new()};
    if (!ctx) {
        throw std::runtime_error("EVP_MD_CTX_new failed");
    }

    if (!EVP_DigestInit_ex(ctx.get(), EVP_sha256(), nullptr)) {
        throw std::runtime_error("EVP_DigestInit_ex failed");
    }

    constexpr std::size_t kBufferSize{8192};
    std::array<unsigned char, kBufferSize> buf{};
    while (true) {
        const auto n =
            std::fread(buf.data(), 1, buf.size(), file.get());
        if (n == 0) break;
        if (!EVP_DigestUpdate(ctx.get(), buf.data(), n)) {
            throw std::runtime_error("EVP_DigestUpdate failed");
        }
    }

    if (std::ferror(file.get())) {
        throw std::runtime_error(
            std::format("error reading file: {}", filepath));
    }

    std::array<unsigned char, EVP_MAX_MD_SIZE> digest{};
    unsigned digest_len{};
    if (!EVP_DigestFinal_ex(ctx.get(), digest.data(), &digest_len)) {
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    std::string result;
    result.resize(digest_len * 2);
    auto* out = result.data();
    for (unsigned i = 0; i < digest_len; ++i) {
        auto r = std::format_to_n(out, 2, "{:02x}", digest[i]);
        out += r.size;
    }
    return result;
}

} // namespace sha256sum
