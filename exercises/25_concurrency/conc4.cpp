// conc4.cpp        (ThreadSanitizer build — it reports lock inversions!)
//
// DEADLOCK, the classic two-account transfer:
//
//     thread A: lock(alice), then lock(bob)      — transfer a→b
//     thread B: lock(bob),  then lock(alice)     — transfer b→a
//
// A holds alice and waits for bob; B holds bob and waits for alice.
// Forever. This file deadlocks DETERMINISTICALLY (a sleep between the
// two acquisitions guarantees the fatal interleaving) — the runner's
// 30-second timeout will call it.
//
// Two industrial fixes:
//
//   1. A GLOBAL LOCK ORDER — every transfer locks the lower-numbered
//      account first, regardless of direction. Convention-based; works;
//      audits poorly.
//
//   2. Ask the library to take BOTH locks in one breath. C++17 added
//      a lock type for exactly this — hand it every mutex you need
//      and it acquires them all with a deadlock-avoidance algorithm,
//      atomically from your perspective. This scenario is the reason
//      that type exists.
//
// Task: make transfer() deadlock-free.
//   - the program finishes (no 30-second timeout) and both asserts
//     pass: alice 80, bob 120
//   - runs clean under TSan — its lock-inversion report gone too
// Constraints:
//   - keep the sleep: your fix must survive the window, not shrink it
//   - each Account keeps its own mutex, and both accounts stay
//     protected while the balances move — no single global lock

#include <cassert>
#include <chrono>
#include <mutex>
#include <thread>

struct Account {
    int balance = 0;
    std::mutex m;
};

void transfer(Account& from, Account& to, int amount) {
    std::lock_guard first(from.m);
    // Window of doom — wide enough that both threads always enter it:
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::lock_guard second(to.m);

    from.balance -= amount;
    to.balance += amount;
}

int main() {
    Account alice{.balance = 100};
    Account bob{.balance = 100};

    {
        std::jthread t1(transfer, std::ref(alice), std::ref(bob), 30);
        std::jthread t2(transfer, std::ref(bob), std::ref(alice), 10);
    }

    assert(alice.balance == 80);
    assert(bob.balance == 120);
    return 0;
}

// I AM NOT DONE
