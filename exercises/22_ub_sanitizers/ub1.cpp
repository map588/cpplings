// ub1.cpp
//
// Signed integer overflow is UB — not "wraps around", UB. The optimizer
// USES that: it folds `x + 1 > x` to true, hoists loop bounds, deletes
// "impossible" branches. Code that "worked" with wrapped values gets
// quietly rewritten at -O2 into code that assumes the wrap never
// happened. (Unsigned is different: defined to wrap, module 01.)
//
// Today's specimen is FAMOUS: the binary-search midpoint. This exact
// bug lived in the JDK and countless textbooks for decades:
//
//     int mid = (lo + hi) / 2;     // lo + hi can exceed INT_MAX
//
// With lo and hi both ~1.5 billion (indices into a big array — easily
// real on modern data), lo + hi overflows. UBSan reports the exact
// line; without UBSan you'd get a negative mid and a wild array read.
//
// The classic fix computes the midpoint WITHOUT a sum that can grow:
//
//     int mid = lo + (hi - lo) / 2;      // hi >= lo, so hi-lo fits
//
// Task: fix midpoint(). (Alternative honorable fixes: widen to int64_t,
// or use std::midpoint — yes, C++20 added it BECAUSE of this bug.)

#include <cassert>
#include <climits>

int midpoint(int lo, int hi) {     // precondition: lo <= hi
    return (lo + hi) / 2;
}

int main() {
    assert(midpoint(0, 10) == 5);
    assert(midpoint(10, 20) == 15);

    // Two large indices into a hypothetical 2-billion-element dataset:
    int lo = 1'500'000'000;
    int hi = 2'000'000'000;
    assert(midpoint(lo, hi) == 1'750'000'000);
    return 0;
}

// I AM NOT DONE
