#ifndef SHA256SUM_HPP
#define SHA256SUM_HPP

#include <string>
#include <string_view>

namespace sha256sum {

auto hash_file(std::string_view filepath) -> std::string;

} // namespace sha256sum

#endif // SHA256SUM_HPP
