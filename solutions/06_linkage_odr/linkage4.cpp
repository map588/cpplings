// linkage4.cpp — solution (the fix is in linkage4_util.h)

#include "linkage4_util.h"

#include <cassert>

double mean_clamped(const int* xs, int n);

int main() {
    assert(clamp01(-5) == 0);
    assert(clamp01(99) == 1);

    int xs[] = {-5, 1, 99, 0};
    assert(mean_clamped(xs, 4) == 0.5);
    return 0;
}
