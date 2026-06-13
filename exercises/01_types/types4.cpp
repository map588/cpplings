// types4.cpp
//
// The most common real-world signed/unsigned bug, in its natural habitat.
//
// .size() returns std::size_t — an UNSIGNED type. When the container is
// empty, `v.size() - 1` does not produce -1. It wraps around to
// 18446744073709551615 (on 64-bit), and the loop below runs "forever",
// reading memory the vector doesn't own. AddressSanitizer will catch the
// very first bad read — watch it happen.
//
// Related trap: `i < v.size()` where i is int draws a -Wsign-compare
// warning, because the int gets converted to unsigned for the comparison
// (so i = -1 would compare as a huge number).
//
// Modern tools for this (both C++20):
//   std::ssize(v)            — size as a SIGNED type
//   std::cmp_less(a, b)      — mathematically correct mixed-sign comparison
//
// Task: fix sum_adjacent_pairs so it's correct for empty input. The classic
// fix needs no new features at all — rearrange the comparison so nothing
// can underflow.


#include <cassert>
#include <cstddef>
#include <vector>

// Sums v[i] + v[i+1] for every adjacent pair.
int sum_adjacent_pairs(const std::vector<int>& v) {
    int total = 0;
    for (std::size_t i = 0; i < v.size() - 1; ++i) {
        total += v[i] + v[i + 1];
    }
    return total;
}

int main() {
    std::vector<int> empty;
    assert(sum_adjacent_pairs(empty) == 0);  // boom — underflow, wild reads

    std::vector<int> v = {1, 2, 3};
    assert(sum_adjacent_pairs(v) == 8);      // (1+2) + (2+3)
    return 0;
}
// I AM NOT DONE
