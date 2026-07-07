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
// below is dead code in every instantiation — the compilers even warn
// about it (read the build output!).
//
// The repair follows from the diagnosis: the question must be asked
// when the function actually RUNS, not while its branches are being
// selected at compile time. Both branches must survive to codegen;
// the optimizer will still fold the check away at runtime. The fix
// DELETES more than it adds. (C++23 closed the trap with dedicated
// syntax: `if consteval { }`.)
//
// Task: make square() report the path that actually computed it.
//   - static_assert(square(12) == 144) still compiles and passes
//   - the runtime call takes the runtime path: mode ends up 'r' and
//     both runtime asserts pass
// Constraints:
//   - one function serving both worlds — no overloads, no second
//     function, no consteval/constinit tricks
//   - keep the mode-reporting logic and every assert

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

// I AM NOT DONE
