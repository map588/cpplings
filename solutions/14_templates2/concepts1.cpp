// concepts1.cpp — solution

#include <cassert>
#include <concepts>
#include <vector>

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T>
T average(const std::vector<T>& v) {
    T sum{};
    for (const T& x : v) sum += x;
    return sum / static_cast<T>(v.size());
}

int main() {
    std::vector<int> ints = {2, 4, 6};
    assert(average(ints) == 4);

    std::vector<double> reals = {1.0, 2.0};
    assert(average(reals) == 1.5);

    // average(words) now fails with:
    //   "no matching function ... constraints not satisfied ...
    //    'std::string' does not satisfy 'Numeric'"
    // — at the call site, in the caller's vocabulary.
    return 0;
}
