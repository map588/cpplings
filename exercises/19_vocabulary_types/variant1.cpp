// variant1.cpp
//
// std::variant<A, B, C> (C++17): exactly ONE of the alternatives is
// alive, and the variant KNOWS WHICH — a tagged union where the
// language manages the tag. (Raw C unions make YOU track the active
// member; reading the wrong one is UB. The variant turns that UB into
// a thrown exception or a null pointer.)
//
// The accessors:
//   std::holds_alternative<T>(v)   — is T the active one?
//   std::get<T>(v)                 — the value; WRONG T → throws
//                                    std::bad_variant_access
//   std::get_if<T>(&v)             — pointer to the value, or nullptr.
//                                    The check-and-access in one step.
//   v.index()                      — which alternative, by position
//
// The shipped describe() assumes every input is a number and get<int>'s
// its way into an exception the moment a word arrives.
//
// Task: make describe() correct for every alternative Input can hold.
//   - the program compiles and every assert passes
//   - describe() never throws, whatever the active alternative
// Constraints:
//   - do not change parse_input, main, or the Input alias
//   - no try/catch — check before you reach, or reach in a way that
//     cannot fail

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
    // Hope is not a strategy:
    return "number " + std::to_string(std::get<int>(input));
}

int main() {
    Input n = parse_input("42");
    assert(std::holds_alternative<int>(n));
    assert(n.index() == 0);

    Input w = parse_input("hello");
    assert(std::holds_alternative<std::string>(w));

    assert(describe(n) == "number 42");
    assert(describe(w) == "word hello");     // throws today
    return 0;
}

// I AM NOT DONE
