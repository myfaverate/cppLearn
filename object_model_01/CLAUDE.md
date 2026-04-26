# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

```bash
# Configure and build (from repo root)
cmake -S . -B build
cmake --build build

# Build with parallel jobs
cmake --build build -j$(nproc)

# Clean rebuild
rm -rf build && cmake -S . -B build && cmake --build build -j$(nproc)
```

## Project Architecture

This is a C++26 learning project exploring C++ object model details. It builds as a single executable (`object_model_01`).

**Entry point** (`main.cpp`): A CLI tool that computes and prints the SHA256 hash of a given file. Throws to an `std::cerr` catch on error.

**Source layout** — `learn01` and `learn02` are separate learning modules isolated by namespace:
- `include/learn01/` / `src/learn01/` — `learn001` namespace (basic experiments)
- `include/learn02/` / `src/learn02/` — `sha256sum` namespace (file hashing via OpenSSL EVP API)

**Key conventions:**
- Uses trailing return type syntax (`auto fn() -> int`)
- C++26 features in use: `std::format`, lambda in template argument (for `unique_ptr` deleter)
- OpenSSL errors are handled with manual `EVP_MD_CTX_free` before throw (RAII-incomplete for the digest context)
- Strict compiler flags: `-Wall -Werror -Wextra -pedantic`, NDKDEBUG defined in all configs
- Header guard style: `#ifndef NAME_HPP` / `#define NAME_HPP`
