// namespaces2.cpp — solution

#include <cassert>
#include <string>

namespace json {
    std::string serialize(int v) { return "{" + std::to_string(v) + "}"; }
    constexpr int indent_width = 2;
}

namespace yaml {
    std::string serialize(int v) { return "- " + std::to_string(v); }
    constexpr int indent_width = 4;
}

// Using-declarations: import exactly the names we mean.
using json::serialize;
using yaml::indent_width;

int main() {
    assert(serialize(7) == "{7}");
    assert(indent_width == 4);
    assert(json::indent_width == 2);    // qualification always works
    return 0;
}
