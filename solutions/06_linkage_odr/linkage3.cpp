// linkage3.cpp — solution

#include <cassert>
#include <string>

namespace {  // internal linkage: this TU's private normalize
std::string normalize(std::string s) {
    for (char& c : s) {
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
    }
    return s;
}
}  // namespace

std::string render_header(std::string title);

int main() {
    assert(normalize("MiXeD") == "mixed");
    assert(render_header("intro") == "== INTRO ==");
    return 0;
}
