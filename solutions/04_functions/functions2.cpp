// functions2.cpp — solution

#include <cassert>
#include <string>

int ids_handed_out = 0;
int next_id() { return ++ids_handed_out; }

// Required parameter first, defaulted parameter last.
std::string make_tag(int id, std::string prefix = "tag") {
    return prefix + std::to_string(id);
}

std::string auto_tag(int id = next_id()) {
    return "#" + std::to_string(id);
}

int main() {
    assert(make_tag(7) == "tag7");
    assert(make_tag(8, "user") == "user8");

    assert(auto_tag() == "#1");
    assert(auto_tag() == "#2");
    assert(auto_tag(99) == "#99");

    // The default ran only for the two calls that omitted the argument.
    assert(ids_handed_out == 2);
    return 0;
}
