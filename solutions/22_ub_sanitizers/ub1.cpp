// ub1.cpp — solution

#include <cassert>
#include <climits>

int midpoint(int lo, int hi) {
    return lo + (hi - lo) / 2;     // no sum that can exceed INT_MAX
    // also correct: std::midpoint(lo, hi)  (<numeric>, C++20)
}

int main() {
    assert(midpoint(0, 10) == 5);
    assert(midpoint(10, 20) == 15);

    int lo = 1'500'000'000;
    int hi = 2'000'000'000;
    assert(midpoint(lo, hi) == 1'750'000'000);
    return 0;
}
