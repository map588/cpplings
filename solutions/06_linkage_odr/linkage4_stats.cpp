// linkage4_stats.cpp — unchanged.

#include "linkage4_util.h"

double mean_clamped(const int* xs, int n) {
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += clamp01(xs[i]);
    }
    return sum / n;
}
