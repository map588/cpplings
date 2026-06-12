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
//     if constexpr (std::is_pointer_v<T>) {
//         ... *value ...                  // only exists when T is a pointer
//     } else {
//         ...                             // only exists otherwise
//     }
//
// One function body, per-type behavior, no specialization ladder, no
// overload set, no SFINAE. This (plus concepts) is what retired most
// C++11-era template metaprogramming tricks.
//
// The condition must be a compile-time constant — type traits
// (<type_traits>, C++11; the _v shorthands are C++17) are the usual
// fuel: std::is_pointer_v, std::is_same_v, or any concept used as a
// bool, like std::integral<T>.
//
// Task: one keyword (twice).

// I AM NOT DONE

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
