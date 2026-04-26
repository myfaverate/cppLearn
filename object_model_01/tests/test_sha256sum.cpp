#include "sha256sum.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

static int g_failed = 0;
static int g_passed = 0;

#define TEST(name)                                                             \
    static void test_##name();                                                 \
    struct Register_##name {                                                   \
        Register_##name() {                                                    \
            std::cout << "  RUN    " << #name << '\n';                         \
            try {                                                              \
                test_##name();                                                 \
                std::cout << "  PASSED " << #name << '\n';                     \
                ++g_passed;                                                    \
            } catch (const std::exception& e) {                                \
                std::cout << "  FAILED " << #name << ": " << e.what() << '\n'; \
                ++g_failed;                                                    \
            }                                                                  \
        }                                                                      \
    } register_##name;                                                         \
    static void test_##name()

#define ASSERT_EQ(a, b)                                                        \
    do {                                                                       \
        if ((a) != (b)) {                                                      \
            throw std::runtime_error(                                          \
                std::string("ASSERT_EQ failed: ") + #a + " != " + #b);         \
        }                                                                      \
    } while (0)

#define ASSERT_THROWS(expr)                                                    \
    do {                                                                       \
        bool caught = false;                                                   \
        try {                                                                  \
            expr;                                                              \
        } catch (...) {                                                        \
            caught = true;                                                     \
        }                                                                      \
        if (!caught) {                                                         \
            throw std::runtime_error(                                          \
                "ASSERT_THROWS failed: no exception thrown for " #expr);       \
        }                                                                      \
    } while (0)

static auto write_temp_file(const std::string& path, std::string_view content)
    -> void {
    std::ofstream f(path, std::ios::binary);
    f.write(content.data(), static_cast<std::streamsize>(content.size()));
}

TEST(empty_file) {
    const auto* path = "/tmp/sha256_test_empty.bin";
    write_temp_file(path, "");
    auto hash = sha256sum::hash_file(path);
    ASSERT_EQ(
        hash,
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    std::remove(path);
}

TEST(known_string_abc) {
    const auto* path = "/tmp/sha256_test_abc.txt";
    write_temp_file(path, "abc");
    auto hash = sha256sum::hash_file(path);
    ASSERT_EQ(
        hash,
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    std::remove(path);
}

TEST(known_string_hello_world) {
    const auto* path = "/tmp/sha256_test_hello.txt";
    write_temp_file(path, "hello world");
    auto hash = sha256sum::hash_file(path);
    ASSERT_EQ(
        hash,
        "b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9");
    std::remove(path);
}

TEST(file_not_found) {
    ASSERT_THROWS(sha256sum::hash_file("/tmp/nonexistent_sha256_test_xyz"));
}

TEST(larger_file) {
    const auto* path = "/tmp/sha256_test_large.bin";
    {
        std::ofstream f(path, std::ios::binary);
        for (int i = 0; i < 1000; ++i) {
            f.write("The quick brown fox jumps over the lazy dog.\n", 44);
        }
    }
    auto hash = sha256sum::hash_file(path);
    ASSERT_EQ(hash.length(), 64U);
    // All hex characters
    for (auto c : hash) {
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
            throw std::runtime_error("hash contains non-hex characters");
        }
    }
    std::remove(path);
}

auto main() -> int {
    std::cout << "sha256sum tests:\n";
    // Test registration happens via static constructors before main
    std::cout << "\nResults: " << g_passed << " passed, " << g_failed
              << " failed\n";
    return g_failed > 0 ? 1 : 0;
}
