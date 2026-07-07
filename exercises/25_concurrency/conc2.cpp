// conc2.cpp        (built with ThreadSanitizer — see exercises.toml)
//
// `++hits` is not one operation. It's three: load, add, store. Two
// threads interleaving them:
//
//     T1 loads 41        T2 loads 41
//     T1 stores 42       T2 stores 42      ← one increment EVAPORATED
//
// That's a DATA RACE — and per the standard it's not "lost updates",
// it's undefined behavior outright. ThreadSanitizer catches the
// conflicting access pair with both stack traces; run this and read
// the report (this module's exercises swap ASan for TSan — they can't
// be combined).
//
// For a lone counter, C++11 ships a dedicated wrapper type that makes
// ++ one indivisible read-modify-write — race gone, no lock needed.
// Know its limits before falling in love: it fixes SINGLE-OBJECT
// races. An invariant across two variables, or a container, needs a
// mutex (conc3) — two race-free objects do not make their COMBINATION
// race-free.
//
// Task: make the counter race-free.
//   - the assert passes: exactly 200'000 — every increment counted
//   - runs clean under TSan (no data-race report)
// Constraints:
//   - no mutex — a lone counter deserves the lighter tool
//   - keep both threads, the loop counts, and the assert

#include <cassert>
#include <thread>

int hits = 0;

void hammer() {
    for (int i = 0; i < 100'000; ++i) {
        ++hits;
    }
}

int main() {
    {
        std::jthread a(hammer);
        std::jthread b(hammer);
    }

    assert(hits == 200'000);     // some increments never happened
    return 0;
}

// I AM NOT DONE
