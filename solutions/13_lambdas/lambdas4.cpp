// lambdas4.cpp — solution

#include <cassert>
#include <vector>

int main() {
    std::vector<int> prices;
    prices.push_back(99);

    // Value init-capture: a snapshot, immune to reallocation.
    auto launch_price = [first = prices[0]]() { return first; };

    for (int p = 100; p < 200; ++p) {
        prices.push_back(p);
    }

    assert(launch_price() == 99);
    assert(prices.size() == 101);
    return 0;
}
