// lambdas4.cpp
//
// A by-reference capture is a stored reference — and a stored reference
// keeps NOTHING alive (module 03). The closure outliving what it
// captured is THE lambda bug: callbacks capturing locals, [&this],
// handlers registered today and fired tomorrow.
//
// Here's a sneaky habitat. The lambda captures a reference to
// `prices[0]`. Then the vector GROWS — push_back reallocates the
// buffer, the element moves to a new address, and the closure's
// reference points into freed memory. AddressSanitizer catches the read
// (heap-use-after-free) — note the free happens inside vector, far from
// any visible delete.
//
// Capture-choice rules of thumb:
//   - the closure outlives the current scope (stored, returned,
//     async)? → capture by VALUE, or move what you need in:
//     [snapshot = prices[0]]
//   - the closure is consumed before the scope ends (passed straight
//     to std::sort, count_if)? → [&] is fine and cheap
//   - container ELEMENTS are extra treacherous: even if the container
//     lives on, its elements move (module 16: iterator invalidation)
//
// Task: cheapest_first should remember the FIRST price as a value, not
// chase the element's address.

#include <cassert>
#include <vector>

int main() {
    std::vector<int> prices;
    prices.push_back(99);

    // Snapshot the launch price — to compare against later:
    auto launch_price = [&first = prices[0]]() { return first; };

    // Business booms; the vector reallocates somewhere in here:
    for (int p = 100; p < 200; ++p) {
        prices.push_back(p);
    }

    assert(launch_price() == 99);          // reads freed memory
    assert(prices.size() == 101);
    return 0;
}

// I AM NOT DONE
