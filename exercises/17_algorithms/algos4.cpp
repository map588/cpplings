// algos4.cpp
//
// std::transform: map a function over a range.
//
//     std::transform(src.begin(), src.end(),     // input range
//                    dest_first,                  // where output goes
//                    [](T x) { return f(x); });
//
// The trap is `dest_first`: transform WRITES THROUGH it, assuming the
// room already exists. Pointing it at an empty vector's begin() writes
// into nothing (the shipped bug — ASan will object).
//
// Two correct destinations:
//   - pre-sized:    dest.resize(src.size()); ... dest.begin()
//   - grow-on-write: std::back_inserter(dest)   (<iterator>) — an
//     iterator whose every write calls dest.push_back. (reserve()
//     first if you care about reallocations; module 16.)
//
// Then accumulate's overload with a BINARY OP folds a range through
// any combiner — here, "running maximum":
//
//     accumulate(f, l, init, [](acc, x) { return combine(acc, x); })
//
// Task: fix fahrenheit() and implement hottest().
//   - fahrenheit() runs ASan-clean and returns all three converted
//     values (either honest destination works — the asserts only check
//     the values)
//   - hottest() returns the maximum of temps, but never less than
//     `floor` — note the second assert: the floor wins over a cooler
//     day
// Constraints:
//   - no for/while/manual loops in either function
//   - hottest() is a single algorithm call — `floor` is not a special
//     case to branch on, it's a starting value
//   - don't change the asserts

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <vector>

std::vector<double> fahrenheit(const std::vector<double>& celsius) {
    std::vector<double> out;
    std::transform(celsius.begin(), celsius.end(),
                   out.begin(),                       // writes into nothing!
                   [](double c) { return c * 9.0 / 5.0 + 32.0; });
    return out;
}

double hottest(const std::vector<double>& temps, double floor) {
    return 0;   // TODO
}

int main() {
    std::vector<double> c = {0.0, 100.0, 25.0};
    std::vector<double> f = fahrenheit(c);

    assert(f.size() == 3);
    assert(f[0] == 32.0);
    assert(f[1] == 212.0);
    assert(f[2] == 77.0);   // all three chosen exact in binary

    assert(hottest({3.5, 9.25, 7.0}, 0.0) == 9.25);
    assert(hottest({3.5, 9.25}, 50.0) == 50.0);   // floor wins
    return 0;
}

// I AM NOT DONE
