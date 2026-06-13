// references2.cpp
//
// Binding rules:
//
//   int& r = 5;          // ERROR: non-const ref can't bind a temporary.
//                        // (If it could, you'd be mutating a value with
//                        // no home — what would that even mean?)
//   const int& r = 5;    // OK! And — the special rule — the temporary's
//                        // LIFETIME IS EXTENDED to match r's scope.
//
// Lifetime extension is why this everyday code is safe:
//
//   const std::string& s = make_string();   // temporary lives on as s
//
// ...but it does NOT chain through functions or members: extension applies
// only when the reference binds the temporary DIRECTLY. A reference
// returned from a function that received a temporary is dangling.
//
// This rule is also why `const T&` parameters accept literals:
// `lookup("hi")` works because the materialized temporary binds to the
// const reference for the duration of the call.
//
// Task: two lines don't compile. Fix them by choosing the right kind of
// reference — don't copy into new variables.

#include <cassert>
#include <string>

std::string make_greeting() {
    return "hello";              // returns a temporary
}

// Counts vowels. Read-only — but declared so literals can't bind to it.
std::size_t count_vowels(std::string& text) {
    std::size_t n = 0;
    for (char c : text) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') ++n;
    }
    return n;
}

int main() {
    // Binds a temporary, keeps it alive for the whole of main():
    std::string& greeting = make_greeting();
    assert(greeting == "hello");

    assert(count_vowels(greeting) == 2);
    assert(count_vowels("aeiou") == 5);   // a literal makes a temporary
    return 0;
}

// I AM NOT DONE
