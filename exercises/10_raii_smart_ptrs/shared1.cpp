// shared1.cpp
//
// std::shared_ptr = shared ownership: the LAST owner standing deletes.
// The bookkeeping lives in a heap-allocated CONTROL BLOCK (reference
// count + weak count + deleter) that all owners point to. Know the costs:
//
//   - a shared_ptr is TWO pointers wide (object + control block):
//         static_assert(sizeof(std::shared_ptr<int>) == 2*sizeof(void*));
//     (every mainstream implementation; unique_ptr is one)
//   - COPYING bumps the count — an ATOMIC increment, so it's thread-safe
//     and not free. MOVING transfers without touching the count.
//   - std::shared_ptr<T>(new T) performs TWO allocations: the T, then
//     the control block. std::make_shared<T>() fuses them into ONE.
//
// Default to unique_ptr; reach for shared_ptr when ownership genuinely
// has no single home — not "I couldn't decide who owns it."
//
// Task: predict every use_count. Then fix the hand-off at the bottom of
// main — it was meant to GIVE the registry ownership, not share it.

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

constexpr long TODO = -1;

std::vector<std::shared_ptr<int>> registry;

void transfer(std::shared_ptr<int> p) {
    registry.push_back(std::move(p));
}

int main() {
    auto a = std::make_shared<int>(42);          // one allocation, count 1
    assert(a.use_count() == TODO);

    {
        std::shared_ptr<int> b = a;              // copy: atomic ++
        assert(a.use_count() == TODO);

        std::shared_ptr<int> c = std::move(b);   // move: count unchanged
        assert(a.use_count() == TODO);
        assert(b == nullptr);                    // moved-from: null
    }                                            // c dies: atomic --

    assert(a.use_count() == TODO);

    // Hand the int to the registry FOR KEEPS — a is supposed to be out
    // of the picture (and the hand-off shouldn't touch the atomic count
    // at all; moves never do):
    transfer(a);                                 // shares instead of gives
    assert(a == nullptr);                        // we meant to let go
    assert(registry[0].use_count() == 1);        // registry: sole owner

    static_assert(sizeof(std::shared_ptr<int>) == 2 * sizeof(void*));
    static_assert(sizeof(std::unique_ptr<int>) == 1 * sizeof(void*));
    return 0;
}

// I AM NOT DONE
