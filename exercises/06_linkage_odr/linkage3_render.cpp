// linkage3_render.cpp — this file ALSO has a private normalize().
// Same name, different meaning: uppercase. Fix this one too.

#include <string>

// This file's idea of "normalize": uppercase.
std::string normalize(std::string s) {
    for (char& c : s) {
        if (c >= 'a' && c <= 'z') c -= 'a' - 'A';
    }
    return s;
}

std::string render_header(std::string title) {
    return "== " + normalize(title) + " ==";
}
