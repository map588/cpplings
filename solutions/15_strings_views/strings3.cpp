// strings3.cpp — solution

#include <cassert>
#include <string>
#include <string_view>

std::string make_header() {
    return "Content-Type-With-Extremely-Verbose-Name: application/json";
}

std::string_view first_word(std::string_view line) {
    return line.substr(0, line.find(':'));
}

int main() {
    // Name the owner; views may then point into it freely.
    std::string header = make_header();
    std::string_view name = first_word(header);

    assert(name == "Content-Type-With-Extremely-Verbose-Name");
    assert(name.size() == 40);
    return 0;
}
