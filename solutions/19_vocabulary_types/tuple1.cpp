// tuple1.cpp — solution

#include <cassert>
#include <tuple>
#include <vector>

std::tuple<int, int, double> stats(const std::vector<int>& v) {
    int lo = v[0];
    int hi = v[0];
    double sum = 0;
    for (int x : v) {
        if (x < lo) lo = x;
        if (x > hi) hi = x;
        sum += x;
    }
    return {lo, hi, sum / static_cast<double>(v.size())};
}

int main() {
    std::vector<int> latencies = {12, 7, 41, 12, 8};

    auto [lo, hi, mean] = stats(latencies);

    assert(lo == 7);
    assert(hi == 41);
    assert(mean == 16.0);

    struct Point { int x, y; };
    Point p{3, 9};
    auto& [px, py] = p;
    px += 1;
    assert(p.x == 4 && py == 9);
    return 0;
}
