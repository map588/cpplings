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
//   2. std::scoped_lock (C++17) — hand it BOTH mutexes at once:
//          std::scoped_lock lk(from.m, to.m);
//      It acquires them with a deadlock-avoidance algorithm, atomically
//      from your perspective. This is the reason scoped_lock exists.
//
// Task: fix transfer() with a single scoped_lock taking both mutexes.

// I AM NOT DONE

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
