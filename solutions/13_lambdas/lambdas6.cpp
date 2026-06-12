// lambdas6.cpp — solution

#include <cassert>
#include <cstddef>
#include <functional>

// Type erasure: holds anything callable as int(int) — including
// stateful closures.
std::function<int(int)> g_handler;

void store_handler(std::function<int(int)> h) {
    g_handler = std::move(h);
}

int main() {
    auto doubler = [](int x) { return 2 * x; };

    int offset = 100;
    auto offsetter = [offset](int x) { return x + offset; };

    static_assert(sizeof(doubler) == 1);     // empty class minimum
    static_assert(sizeof(offsetter) == sizeof(int));

    store_handler(doubler);
    assert(g_handler(21) == 42);

    store_handler(offsetter);                // state rides inside
    assert(g_handler(1) == 101);
    return 0;
}
