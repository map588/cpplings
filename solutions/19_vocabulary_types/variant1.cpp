// variant1.cpp — solution

#include <cassert>
#include <string>
#include <variant>

using Input = std::variant<int, std::string>;

Input parse_input(const std::string& raw) {
    if (!raw.empty() && raw.find_first_not_of("0123456789") == std::string::npos) {
        return std::stoi(raw);
    }
    return raw;
}

std::string describe(const Input& input) {
    if (const int* n = std::get_if<int>(&input)) {
        return "number " + std::to_string(*n);
    }
    return "word " + std::get<std::string>(input);   // only two alternatives
}

int main() {
    Input n = parse_input("42");
    assert(std::holds_alternative<int>(n));
    assert(n.index() == 0);

    Input w = parse_input("hello");
    assert(std::holds_alternative<std::string>(w));

    assert(describe(n) == "number 42");
    assert(describe(w) == "word hello");
    return 0;
}
