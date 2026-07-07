// lambdas2.cpp
//
// The capture list decides WHAT the closure carries and HOW:
//
//   [x]    by VALUE — a copy made AT THE LAMBDA'S CREATION. Later
//          changes to the original are invisible to the closure.
//   [&x]   by REFERENCE — the closure sees (and can change) the
//          original. No copy; also no lifetime safety (lambdas4...).
//
// Defaults exist — [=] copies everything used, [&] references
// everything used — but spelling out each capture documents exactly
// what the closure depends on. Style guides increasingly demand it.
//
// Task: fill in the TODO values, then write record_taking.
//   - all five asserts pass
//   - the two TODO values follow from WHEN each capture happens — work
//     them out, don't run-and-paste
//   - record_taking: takes (int n), subtracts n from stock, returns the
//     remaining stock
// Constraints:
//   - record_taking must change the real `stock` through its capture —
//     the final assert reads the original variable, not the closure
//   - don't move the `price = 250;` line; don't change any assert

#include <cassert>

constexpr int TODO = -1;

int main() {
    int price = 100;

    auto frozen = [price]() { return price; };     // copy, made RIGHT NOW
    auto live   = [&price]() { return price; };    // window onto price

    price = 250;

    assert(frozen() == TODO);
    assert(live() == TODO);

    // ---- part two -----------------------------------------------------
    int stock = 8;

    // TODO: a lambda taking (int n) that SUBTRACTS n from stock and
    // returns the remaining stock. Choose the capture that can mutate
    // the real `stock`.
    auto record_taking = nullptr;

    assert(record_taking(3) == 5);
    assert(record_taking(2) == 3);
    assert(stock == 3);                  // the original must have changed
    return 0;
}

// I AM NOT DONE
