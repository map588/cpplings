// linkage4.cpp        (with linkage4_stats.cpp and linkage4_util.h)
//
// linkage4_util.h defines a small function WITH ITS BODY, and two TUs
// include it. Textual paste → two definitions with external linkage →
// duplicate symbol at link time.
//
// This is what `inline` is actually FOR. Forget "hints the compiler to
// inline calls" — optimizers ignored that decades ago (they inline what
// they like at -O2 regardless). The real meaning:
//
//   inline = "this definition may appear in many TUs; they're all
//             identical; linker, fold them into one."
//
// In other words: inline is PERMISSION TO LIVE IN A HEADER. That's why
// you've never written it on a member function defined inside its class —
// those are implicitly inline. So are constexpr functions (C++11) and
// templates. Only a plain free function defined in a header needs the
// keyword spelled out.
//
// (The "identical in all TUs" part is on YOU — that's the ODR. Same
// header included everywhere = automatically identical = safe.)
//
// Task: fix linkage4_util.h. Two correct one-word answers exist — the
// hint discusses both.

// I AM NOT DONE

#include "linkage4_util.h"

#include <cassert>

double mean_clamped(const int* xs, int n);   // from linkage4_stats.cpp

int main() {
    assert(clamp01(-5) == 0);
    assert(clamp01(99) == 1);

    int xs[] = {-5, 1, 99, 0};               // clamps to 0, 1, 1, 0
    assert(mean_clamped(xs, 4) == 0.5);
    return 0;
}
