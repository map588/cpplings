// conc6.cpp        (ThreadSanitizer build)
//
// The pattern beneath every lock-free queue: PUBLISH DATA WITH A FLAG.
//
//     writer:  payload = 42;  ready = true;
//     reader:  while (!ready) spin;  use(payload);
//
// With a plain bool this is broken TWICE. The flag itself is a data
// race (TSan's report). And even "fixed" with something weaker, both
// compiler and CPU may REORDER: the reader could see ready==true
// while payload's write hasn't reached it yet — using the flag without
// receiving the data it advertises.
//
// The memory model (C++11) fixes both with one upgrade — to the FLAG
// alone: make it an atomic, store it with RELEASE semantics, load it
// with ACQUIRE semantics. Release + acquire creates HAPPENS-BEFORE:
// everything the writer did before its release-store (the payload
// write!) is visible to the reader once its acquire-load sees the
// flag. The flag carries the data's visibility with it. (Plain atomic
// ops default to seq_cst, which includes acquire/release — correct,
// marginally stronger than needed; the explicit orders here are the
// lesson, not an optimization demand.)
//
// Task: make the publication safe.
//   - the assert passes (42 arrives) and the program finishes
//   - runs clean under TSan (no data-race report)
// Constraints:
//   - `payload` stays a plain int — publishing NON-atomic data through
//     an atomic flag is the point of the pattern
//   - no mutex, no cv — this is the flag's job
//   - spell your memory orders explicitly: release on the store,
//     acquire on the load

#include <cassert>
#include <thread>

int payload = 0;
bool ready = false;          // the flag: raced AND reorderable

void writer() {
    payload = 42;            // the data
    ready = true;            // the announcement
}

void reader(int& out) {
    while (!ready) {         // spin until announced
    }
    out = payload;           // safe... if the announcement ordered it
}

int main() {
    int received = 0;
    {
        std::jthread r(reader, std::ref(received));
        std::jthread w(writer);
    }
    assert(received == 42);
    return 0;
}

// I AM NOT DONE
