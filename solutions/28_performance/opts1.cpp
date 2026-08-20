// opts1.cpp — SOLUTION
//
// -O2 defines __OPTIMIZE__ (true), does NOT define NDEBUG (false —
// killing asserts is -DNDEBUG, a separate decision), and does not
// include -ffast-math (false — -O levels never change results). And
// IEEE NaN really is unequal to itself (true).

#include <cassert>
#include <cmath>        // NAN

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
    static_assert(optimizer_on == true);

    // Did -O2 turn our asserts off?
    static_assert(ndebug_defined == false);

    // Is -ffast-math part of -O2? (It never is — it changes RESULTS,
    // and -O levels promise not to.)
    static_assert(fast_math_on == false);

    // The NaN probe -ffast-math would break: IEEE says NaN compares
    // unequal to EVERYTHING, including itself. (This is how isnan
    // works — and why -ffast-math makes it lie.)
    volatile double nan = NAN;            // volatile: no constant-folding
    bool nan_ne_itself = (nan != nan);
    assert(nan_ne_itself == true);
    return 0;
}
