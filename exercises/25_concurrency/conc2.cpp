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
// For a lone counter the fix is std::atomic<int>: ++ becomes an
// indivisible read-modify-write, race gone, no lock needed. Know its
// limits before falling in love: atomics fix SINGLE-OBJECT races.
// An invariant across two variables, or a container, needs a mutex
// (conc3) — two atomics do not make their COMBINATION atomic.
//
// Task: make the counter atomic.

// I AM NOT DONE

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
