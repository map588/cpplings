// ifconstexpr1.cpp
//
// Why won't this compile? to_display(42) never takes the pointer
// branch... at RUNTIME. But a regular `if` compiles BOTH branches for
// every instantiation — and `*value` on an int is nonsense, dead code
// or not. That's the error you're looking at.
//
// `if constexpr` (C++17) decides at COMPILE TIME, and the untaken
// branch is DISCARDED — never instantiated for that T:
//
//     if constexpr (std::is_arithmetic_v<T>) {
//         // this code only EXISTS when the trait holds
//     } else {
//         // and this only when it doesn't
//     }
//
// One function body, per-type behavior, no specialization ladder, no
// overload set, no SFINAE. This (plus concepts) is what retired most
// C++11-era template metaprogramming tricks.
//
// The condition must be a compile-time constant — type traits
// (<type_traits>, C++11; the _v shorthands are C++17) or any concept
// used as a bool are the usual fuel.
//
// Task: make to_display() compile and behave for all four callers.
//   - the int, bool, and int* calls in main() each get the right string
//   - every assert passes, including the nullptr case — a pointer's
//     null-ness is NOT a compile-time fact, so that check must stay a
//     runtime decision
// Constraints:
//   - one function body: no overloads, no specializations, no helpers
//   - don't change the conditions being tested — only how (and when)
//     the branching happens
//   - the fix is one keyword, used twice

#include <cassert>
#include <string>
#include <type_traits>

template <typename T>
std::string to_display(T value) {
    if (std::is_pointer_v<T>) {
        if (value == nullptr) return "(null)";
        return "-> " + std::to_string(*value);
    } else if (std::is_same_v<T, bool>) {
        return value ? "true" : "false";
    } else {
        return std::to_string(value);
    }
}

int main() {
    assert(to_display(42) == "42");
    assert(to_display(true) == "true");

    int x = 7;
    assert(to_display(&x) == "-> 7");
    assert(to_display(static_cast<int*>(nullptr)) == "(null)");
    return 0;
}

// I AM NOT DONE
