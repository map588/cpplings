// inline1_stats.cpp — the second translation unit.
// Including inline1.h compiles mean()'s body into THIS object file
// too. Two object files, two definitions of the same symbol...

#include "inline1.h"

// max - min: how wide the data sits around its mean.
double spread(const double* xs, int n) {
    if (n == 0) {
        return 0.0;
    }
    double lo = xs[0], hi = xs[0];
    for (int i = 1; i < n; ++i) {
        if (xs[i] < lo) { lo = xs[i]; }
        if (xs[i] > hi) { hi = xs[i]; }
    }
    return hi - lo;
}
