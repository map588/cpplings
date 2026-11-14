// conc8.cpp        (ThreadSanitizer build)
//
// conc2 made `++hits` safe with std::atomic and warned: an atomic fixes
// a SINGLE operation. This is the bug on the other side of that
// warning — and TSan will not say a word about it, because nothing
// here is a data race. Every access is atomic. The LOGIC is broken.
//
//     if (tickets.load() > 0) {      // CHECK   — atomic, fine
//         tickets.fetch_sub(1);      // ACT     — atomic, fine
//     }                              // check-THEN-act — not atomic
//
// Two threads can both pass the check before either one acts. Each saw
// a true statement; together they sell one ticket twice. The gap
// between "I looked" and "I wrote" is the bug, and no amount of
// atomic-ness on the two halves closes it.
//
// The tool that closes it is COMPARE-AND-SWAP (CAS), the instruction
// every lock-free structure is built from:
//
//     bool ok = a.compare_exchange_weak(expected, desired);
//
// In ONE indivisible step: if `a` still holds `expected`, store
// `desired` and return true. Otherwise change nothing, return false —
// and write the value `a` actually holds INTO `expected`, so you can
// look again. It says exactly what you need: "write, but only if
// nobody changed it since I looked."
//
// Two things to know before you use it. The _weak form may fail
// "spuriously" — report false although the value matched — which is
// why CAS always lives in a LOOP (the _strong form never does that,
// and costs more on some CPUs). And the loop is the pattern: look,
// decide, try to commit; on failure, decide again with the fresh
// value.
//
// The sleep below holds the gap wide open, so both buyers always step
// into it (conc4's trick): the shipped code sells the single ticket
// twice, every run.
//
// Task: make try_buy() sell each ticket exactly once.
//   - both asserts pass: one sale, and the count never goes negative
//   - runs clean under TSan
// Constraints:
//   - keep the sleep BETWEEN looking at the count and changing it —
//     the fix must survive the gap, not shrink it
//   - no mutex: `tickets` stays a std::atomic<int> and the decision is
//     committed with compare-and-swap
//   - buyer(), main, and the asserts stay as they are

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

std::atomic<int> tickets{1};      // one seat left
std::atomic<int> sold{0};

// Sell one ticket if any is left. Returns true when the sale happened.
bool try_buy() {
    if (tickets.load() > 0) {                                        // look
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // the gap
        tickets.fetch_sub(1);                                        // write
        return true;
    }
    return false;
}

void buyer() {
    if (try_buy()) {
        sold.fetch_add(1);
    }
}

int main() {
    {
        std::jthread a(buyer);
        std::jthread b(buyer);
    }
    assert(sold == 1);        // one ticket, one sale
    assert(tickets == 0);     // and nobody is sitting on seat -1
    return 0;
}

// I AM NOT DONE
