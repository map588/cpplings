// ctad1.cpp
//
// CTAD — class template argument deduction (C++17). Constructor
// arguments can deduce the CLASS template's parameters, the way
// function arguments always could:
//
//     std::pair p{1, 2.5};         // pair<int, double>
//     std::vector v{1, 2, 3};      // vector<int>
//     std::lock_guard lk(mutex);   // the everyday win
//
// Sometimes raw deduction gives the WRONG type. Box("hello") deduces
// T = const char* — a Box of pointer, lifetime problems included
// (module 03). A DEDUCTION GUIDE overrides the conclusion:
//
//     Box(const char*) -> Box<std::string>;
//
// "When constructed from const char*, deduce Box<std::string>." Guides
// live next to the class; the standard library uses exactly this so
// that std::string(it1, it2)-style iterator constructors deduce element
// types correctly.
//
// Task: fill in the two TODO types (what did CTAD deduce?), then add
// the deduction guide that makes the last static_assert true.

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
struct Box {
    T contents;
    explicit Box(T c) : contents(std::move(c)) {}
};

struct TODO_TYPE {};   // replace each use with the deduced type

int main() {
    std::pair p{1, 2.5};
    static_assert(std::is_same_v<decltype(p), TODO_TYPE>);

    std::vector v{1, 2, 3};
    static_assert(std::is_same_v<decltype(v), TODO_TYPE>);

    Box number(42);
    static_assert(std::is_same_v<decltype(number), Box<int>>);

    // Deduces Box<const char*> today — a box of dangling-in-waiting.
    // Write the guide that makes it a Box<std::string>:
    Box greeting("hello");
    static_assert(std::is_same_v<decltype(greeting), Box<std::string>>);
    return 0;
}

// I AM NOT DONE
