// tuple1.cpp
//
// Returning several values, the pre-C++17 catalogue of regret:
// out-parameters (which is in? which is out?), structs-of-the-day, or
// std::pair towers (.first.second.first).
//
// Modern shape: return a std::tuple (C++11), unpack with STRUCTURED
// BINDINGS (C++17):
//
//     std::tuple<int, int, double> stats(...);
//     auto [lo, hi, mean] = stats(v);      // three named locals, sized
//                                          // and typed by the tuple
//
// Bindings unpack more than tuples — anything with a known member list:
//
//     auto [it, inserted] = map.insert(...);      // pair (map2 used it!)
//     for (auto& [key, value] : my_map)           // map elements
//     auto [x, y] = some_struct;                  // plain structs too
//
// Use `auto&` / `const auto&` to bind without copying. And know
// std::tie for the assignment-to-existing-variables case:
//     std::tie(lo, hi) = bounds();      // also: the operator< trick
//
// Task: stats() returns only the minimum. The bindings in main demand
// {min, max, mean} — deliver them. (One pass, no algorithms needed.)

// I AM NOT DONE

#include <cassert>
#include <tuple>
#include <vector>

// Returns {min, max, mean} of a non-empty vector.
int stats(const std::vector<int>& v) {
    int lo = v[0];
    for (int x : v) {
        if (x < lo) lo = x;
    }
    return lo;
}

int main() {
    std::vector<int> latencies = {12, 7, 41, 12, 8};

    auto [lo, hi, mean] = stats(latencies);

    assert(lo == 7);
    assert(hi == 41);
    assert(mean == 16.0);            // (12+7+41+12+8) / 5

    // Bindings on a plain struct, for good measure:
    struct Point { int x, y; };
    Point p{3, 9};
    auto& [px, py] = p;              // references INTO p
    px += 1;
    assert(p.x == 4 && py == 9);
    return 0;
}
