// algos2.cpp — solution

#include <algorithm>
#include <cassert>
#include <vector>

std::size_t drop_zeros(std::vector<int>& readings) {
    // The C++20 way — erase the value, get the count back:
    return std::erase(readings, 0);

    // The classic idiom you'll read in older code:
    //     auto tail = std::remove(readings.begin(), readings.end(), 0);
    //     std::size_t dropped = readings.end() - tail;
    //     readings.erase(tail, readings.end());
    //     return dropped;
}

int main() {
    std::vector<int> readings = {5, 0, 7, 0, 0, 3};

    std::size_t dropped = drop_zeros(readings);

    assert(dropped == 3);
    assert(readings == (std::vector<int>{5, 7, 3}));
    return 0;
}
