// inline1.cpp — SOLUTION (unchanged; the fix lives in the header:
// `inline double mean(...)`. The keyword is an ODR permission slip,
// not a speed knob — inlining itself was always the optimizer's call.)

#include <cassert>
#include "inline1.h"

int main() {
    const double xs[] = {2.0, 4.0, 6.0, 8.0};

    assert(mean(xs, 4) == 5.0);       // uses THIS TU's view of mean()
    assert(spread(xs, 4) == 6.0);     // stats TU used its own view too

    assert(mean(nullptr, 0) == 0.0);
    return 0;
}
