// linkage3_render.cpp — solution

#include <string>

namespace {  // each TU keeps its own private normalize, no clash
std::string normalize(std::string s) {
    for (char& c : s) {
        if (c >= 'a' && c <= 'z') c -= 'a' - 'A';
    }
    return s;
}
}  // namespace

std::string render_header(std::string title) {
    return "== " + normalize(title) + " ==";
}
