// opts1.cpp
//
// (Performance module. New ground rule: these exercises are built
// with -O2, because optimization is the subject. Everything is still
// verified by asserts — never by stopwatch.)
//
// Every other exercise in this course compiled at -O0: the compiler
// translates your code as written. That is a CHOICE, and the flag
// menu looks like this (clang and gcc agree on the names):
//
//     -O0   no optimization; fastest builds, most debuggable  (default!)
//     -Og   optimize, but only ways that don't hurt debugging
//     -O1   basic optimizations
//     -O2   the industry-standard "release" level — vectorization,
//           inlining, the works, without size explosions
//     -O3   -O2 plus more aggressive inlining/unrolling — sometimes
//           faster, sometimes just bigger
//     -Os   optimize for SIZE (embedded's favorite; flash is finite)
//
// And the sharp-edged extras:
//     -march=native   use every instruction THIS cpu has. Binaries
//                     may crash with SIGILL (illegal instruction) on
//                     an older machine — build-farm poison.
//     -flto           link-time optimization: optimize ACROSS .cpp
//                     files at link; inlining stops respecting TU walls.
//     -ffast-math     let FP math break IEEE 754: assume associativity,
//                     assume NaN/Inf never happen. Faster — and `x != x`
//                     stops detecting NaN, so std::isnan may lie.
//
// Two facts people get wrong, both checkable from inside the program:
//   - Optimizers define __OPTIMIZE__, so code can tell.
//   - -O2 does NOT define NDEBUG. Killing asserts is a separate,
//     deliberate act (-DNDEBUG) — build systems bundle it into
//     "Release" mode, the compiler itself never does.
//
// Task: predict the probes. Replace each TODO_BOOL.
// Constraints:
//   - change only the TODO_BOOL values
//   - predict first, run second

#include <cassert>
#include <cmath>        // NAN

constexpr bool TODO_BOOL = false;

#ifdef __OPTIMIZE__
constexpr bool optimizer_on = true;
#else
constexpr bool optimizer_on = false;
#endif

#ifdef NDEBUG
constexpr bool ndebug_defined = true;
#else
constexpr bool ndebug_defined = false;
#endif

#ifdef __FAST_MATH__
constexpr bool fast_math_on = true;
#else
constexpr bool fast_math_on = false;
#endif

int main() {
    // This file is compiled with -O2 (see exercises.toml).
    static_assert(optimizer_on == TODO_BOOL);

    // Did -O2 turn our asserts off?
    static_assert(ndebug_defined == TODO_BOOL);

    // Is -ffast-math part of -O2? (No — it changes RESULTS, and
    // -O0 through -O3 promise not to. The one exception is -Ofast,
    // which is -O3 plus -ffast-math.)
    static_assert(fast_math_on == TODO_BOOL);

    // The NaN probe -ffast-math would break: IEEE says NaN compares
    // unequal to EVERYTHING, including itself. (This is how isnan
    // works — and why -ffast-math makes it lie.)
    volatile double nan = NAN;            // volatile: no constant-folding
    bool nan_ne_itself = (nan != nan);
    assert(nan_ne_itself == TODO_BOOL);
    return 0;
}

// I AM NOT DONE
