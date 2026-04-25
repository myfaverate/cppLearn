
#include <iostream>

#include "HelloWorld.hpp"

auto main(const int argc, const char* argv[]) -> int {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0]
                  << " bsdiff <old_file> <new_file> <patch_file>"
                     " or "
                  << argv[0] << " bspatch <old_file> <new_file> <patch_file>"
                  << std::endl;
        return 1;
    }

    if (argv[1] == std::string("bsdiff")) {
        create_patch(argv[2], argv[3], argv[4]);
    } else {
        apply_patch(argv[2], argv[3], argv[4]);
    }
    return 0;
}