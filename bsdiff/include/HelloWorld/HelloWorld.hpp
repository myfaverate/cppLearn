#ifndef HELLOWORLD_HPP
#define HELLOWORLD_HPP

extern "C" {
#include "bsdiff.h"
#include "bspatch.h"
}

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

void create_patch(const char* old_file, const char* new_file,
                  const char* patch_file);
void apply_patch(const char* old_file, const char* new_file,
                 const char* patch_file);

#endif  // HELLOWORLD_HPP