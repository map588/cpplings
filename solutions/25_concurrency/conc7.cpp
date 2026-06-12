// conc7.cpp — solution

#include <cassert>
#include <future>
#include <numeric>
#include <vector>

long range_sum(const std::vector<int>& v, std::size_t lo, std::size_t hi) {
    long total = 0;
    for (std::size_t i = lo; i < hi; ++i) total += v[i];
    return total;
}

long parallel_sum(const std::vector<int>& v) {
    std::size_t mid = v.size() / 2;

    // Explicit policy: actually concurrent, not maybe-deferred.
    auto front = std::async(std::launch::async,
                            range_sum, std::cref(v), std::size_t{0}, mid);
    long back = range_sum(v, mid, v.size());   // this thread pulls weight too
    return front.get() + back;
}

int main() {
    std::vector<int> data(10'000);
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<int>(i % 7);
    }

    long expected = std::accumulate(data.begin(), data.end(), 0L);
    assert(parallel_sum(data) == expected);

    auto f = std::async(std::launch::async, [] { return 5; });
    assert(f.get() == 5);
    assert(f.valid() == false);   // get() moves the result out: one shot
    return 0;
}
