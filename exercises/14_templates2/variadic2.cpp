// variadic2.cpp
//
// Folds aren't just for arithmetic. The COMMA FOLD runs a statement for
// every pack element, in order:
//
//     (do_something(args), ...);
//
// — that's "expand to do_something(a1), do_something(a2), ..." with the
// comma operator joining them. It's the variadic for-each.
//
// And packs FORWARD (module 09's std::forward, plural form):
//
//     template <typename... Ts>
//     void emplace_all(std::vector<T>& v, Ts&&... args) {
//         (v.push_back(std::forward<Ts>(args)), ...);
//     }
//
// Today's job: a join() that the logging module wants —
//
//     join(", ", 1, "apple", 2.5)   →  "1, apple, 2.5"
//
// Provided: piece(), which stringifies ONE argument. The separator
// logic is the classic trick: prepend the separator before every
// element EXCEPT the first — track "first" with a bool the fold
// mutates.
//
// Task: implement join's body with a comma fold. No recursion.

// I AM NOT DONE

#include <cassert>
#include <string>

std::string piece(const std::string& s) { return s; }
std::string piece(const char* s) { return s; }
std::string piece(char c) { return std::string(1, c); }
template <typename T>
std::string piece(T value) { return std::to_string(value); }

template <typename... Ts>
std::string join(const std::string& sep, Ts... args) {
    // TODO: a comma fold that appends sep (except before the first
    // element) and piece(args) to a result string.
    return "";
}

int main() {
    assert(join(", ", 1, "apple", 2.5) == "1, apple, 2.500000");
    assert(join("-", 2, 0, 2, 6) == "2-0-2-6");
    assert(join("/", "usr") == "usr");
    assert(join(", ") == "");                 // empty pack: empty string
    return 0;
}
