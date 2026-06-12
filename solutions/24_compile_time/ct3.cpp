// ct3.cpp — solution

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <vector>

constexpr int median_of(std::initializer_list<int> values) {
    std::vector<int> v(values);            // compile-time allocation
    std::sort(v.begin(), v.end());         // constexpr since C++20
    return v[v.size() / 2];                // the int escapes; v dies here
}

static_assert(median_of({3, 1, 2}) == 2);
static_assert(median_of({9, 4, 7, 1, 8}) == 7);
static_assert(median_of({42}) == 42);

int main() {
    int sensor_a = 18, sensor_b = 4, sensor_c = 11;
    assert(median_of({sensor_a, sensor_b, sensor_c}) == 11);
    return 0;
}
