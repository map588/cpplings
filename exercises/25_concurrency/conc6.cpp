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
// The memory model (C++11) fixes both with one upgrade:
//
//     std::atomic<bool> ready{false};
//     writer:  ready.store(true, std::memory_order_release);
//     reader:  ready.load(std::memory_order_acquire)
//
// RELEASE on the store + ACQUIRE on the load creates HAPPENS-BEFORE:
// everything the writer did before the release (the payload write!) is
// visible to the reader after its acquire. The flag carries the data's
// visibility with it. (Plain atomic ops default to seq_cst, which
// includes acquire/release — correct, marginally stronger than needed;
// explicit orders here are the lesson, not an optimization demand.)
//
// Task: make `ready` atomic with release/acquire (payload itself can
// stay a plain int — that's the point of the pattern).

// I AM NOT DONE

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
