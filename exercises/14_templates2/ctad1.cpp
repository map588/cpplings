// ctad1.cpp
//
// CTAD — class template argument deduction (C++17). Constructor
// arguments can deduce the CLASS template's parameters, the way
// function arguments always could:
//
//     std::pair p{1, 2.5};         // deduced from the arguments
//     std::vector v{1, 2, 3};      // deduced from the arguments
//     std::lock_guard lk(mutex);   // the everyday win
//
// Sometimes raw deduction gives the WRONG type. A DEDUCTION GUIDE,
// written at namespace scope right after the class, overrides the
// conclusion. The shape is: constructor-ish pattern on the left, arrow,
// verdict on the right —
//
//     template <typename It>
//     Range(It, It) -> Range<typename It::value_type>;
//     // "constructed from two iterators, deduce the element type"
//
// The standard library uses exactly this so that iterator-pair
// constructors deduce element types correctly.
//
// Task: fill in the two TODO types, then write the deduction guide the
// last static_assert demands.
//   - replace each TODO_TYPE use with the full type CTAD deduced there
//   - `Box greeting("hello")` currently deduces a Box of pointer — a
//     box of dangling-in-waiting (module 03). Make that same line
//     deduce Box<std::string> instead
//   - everything compiles; all static_asserts pass
// Constraints:
//   - don't write explicit template arguments at any construction site
//   - don't change Box's constructor or any static_assert line

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

    // Deduces Box<const char*> today — see the Task block:
    Box greeting("hello");
    static_assert(std::is_same_v<decltype(greeting), Box<std::string>>);
    return 0;
}

// I AM NOT DONE
