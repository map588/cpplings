// variadic2.cpp — solution

#include <cassert>
#include <string>

std::string piece(const std::string& s) { return s; }
std::string piece(const char* s) { return s; }
std::string piece(char c) { return std::string(1, c); }
template <typename T>
std::string piece(T value) { return std::to_string(value); }

template <typename... Ts>
std::string join(const std::string& sep, Ts... args) {
    std::string out;
    bool first = true;
    // For each element: separator (unless first), then the piece.
    ((out += (first ? "" : sep), out += piece(args), first = false), ...);
    return out;
}

int main() {
    assert(join(", ", 1, "apple", 2.5) == "1, apple, 2.500000");
    assert(join("-", 2, 0, 2, 6) == "2-0-2-6");
    assert(join("/", "usr") == "usr");
    assert(join(", ") == "");
    return 0;
}
