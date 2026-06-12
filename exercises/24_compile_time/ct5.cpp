// ct5.cpp
//
// std::is_constant_evaluated() (C++20) answers "is this call happening
// inside constant evaluation?" — letting one function take a
// compiler-friendly path at build time and a fast intrinsic path at
// runtime.
//
// And it carries C++20's most famous footgun, which this file has
// stepped on:
//
//     if constexpr (std::is_constant_evaluated())   // ALWAYS TRUE!
//
// An `if constexpr` condition is ITSELF evaluated at compile time — so
// the question "am I being constant-evaluated?" is being asked DURING
// constant evaluation, and the answer is always yes. The runtime branch
// below is dead code in every instantiation. clang even has a warning
// for it (-Wconstant-evaluated — read it in the build output!).
//
// The correct spelling is a PLAIN if: codegen sees both branches, the
// constant evaluator takes one, runtime takes the other, and the
// optimizer deletes the compile-time branch from the emitted code
// (the condition folds to false at runtime).
//
// (C++23 closed the trap with dedicated syntax: `if consteval { }`.)
//
// Task: one word to delete. The mode counters tell the truth.

// I AM NOT DONE

#include <cassert>
#include <type_traits>

// Returns x*x and reports which path computed it: 'c' for the constant
// evaluator, 'r' for the runtime path (pretend it's a SIMD intrinsic).
constexpr int square(int x, char* mode = nullptr) {
    if constexpr (std::is_constant_evaluated()) {
        if (mode) *mode = 'c';
        return x * x;                  // the patient, portable path
    } else {
        if (mode) *mode = 'r';
        return x * x;                  // the "fast intrinsic" path
    }
}

int main() {
    // Compile time — must use the 'c' path (and does, trivially):
    static_assert(square(12) == 144);

    // Runtime — must use the 'r' path... does it?
    char mode = '?';
    int runtime_x = 5;
    assert(square(runtime_x, &mode) == 25);
    assert(mode == 'r');
    return 0;
}
