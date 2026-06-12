// algos4.cpp — solution

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <vector>

std::vector<double> fahrenheit(const std::vector<double>& celsius) {
    std::vector<double> out;
    out.reserve(celsius.size());                  // size known: no regrowth
    std::transform(celsius.begin(), celsius.end(),
                   std::back_inserter(out),       // each write: push_back
                   [](double c) { return c * 9.0 / 5.0 + 32.0; });
    return out;
}

double hottest(const std::vector<double>& temps, double floor) {
    return std::accumulate(temps.begin(), temps.end(), floor,
                           [](double a, double b) { return std::max(a, b); });
}

int main() {
    std::vector<double> c = {0.0, 100.0, 25.0};
    std::vector<double> f = fahrenheit(c);

    assert(f.size() == 3);
    assert(f[0] == 32.0);
    assert(f[1] == 212.0);
    assert(f[2] == 77.0);   // all three chosen exact in binary

    assert(hottest({3.5, 9.25, 7.0}, 0.0) == 9.25);
    assert(hottest({3.5, 9.25}, 50.0) == 50.0);
    return 0;
}
