// types4.cpp — solution

#include <cassert>
#include <cstddef>
#include <vector>

int sum_adjacent_pairs(const std::vector<int>& v) {
    int total = 0;
    // i + 1 < v.size() can't underflow; size() - 1 can.
    for (std::size_t i = 0; i + 1 < v.size(); ++i) {
        total += v[i] + v[i + 1];
    }
    return total;
}

int main() {
    std::vector<int> empty;
    assert(sum_adjacent_pairs(empty) == 0);

    std::vector<int> v = {1, 2, 3};
    assert(sum_adjacent_pairs(v) == 8);
    return 0;
}
