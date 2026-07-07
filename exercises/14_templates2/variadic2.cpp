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
// Provided: piece(), which stringifies ONE argument. The classic
// difficulty is the separator: it goes BETWEEN elements, so one element
// must be treated differently from all the others. A fold can't branch
// on "am I first?" by itself — but the statement it expands can read
// and update state living in the enclosing scope.
//
// Task: implement join()'s body around a comma fold.
//   - every assert passes, including the one-element and zero-element
//     calls
// Constraints:
//   - one comma fold does all the iteration: no recursion, no loops
//   - don't change piece() or the asserts

#include <cassert>
#include <string>

std::string piece(const std::string& s) { return s; }
std::string piece(const char* s) { return s; }
std::string piece(char c) { return std::string(1, c); }
template <typename T>
std::string piece(T value) { return std::to_string(value); }

template <typename... Ts>
std::string join(const std::string& sep, Ts... args) {
    // TODO: build the result here with a comma fold.
    return "";
}

int main() {
    assert(join(", ", 1, "apple", 2.5) == "1, apple, 2.500000");
    assert(join("-", 2, 0, 2, 6) == "2-0-2-6");
    assert(join("/", "usr") == "usr");
    assert(join(", ") == "");                 // empty pack: empty string
    return 0;
}

// I AM NOT DONE
