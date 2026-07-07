// functions3.cpp
//
// `static` inside a function means something completely different from
// `static` anywhere else (C++ reuses this keyword shamelessly):
//
//   A static LOCAL is initialized ONCE — the first time execution reaches
//   the line — and lives until program exit. Every later call sees the
//   same object. Two properties worth internalizing:
//
//   - The initialization is LAZY. A function-local `static Config cfg =
//     load();` doesn't run load() at startup, only on first call. This is
//     the standard fix for the "static initialization order fiasco"
//     (globals in different .cpp files initialize in unspecified order;
//     a function-local static can't be used before it's initialized).
//
//   - Since C++11 that first-time initialization is THREAD-SAFE: if two
//     threads race into the function, one initializes, the other waits.
//     ("magic statics" — the compiler emits a guard for you.)
//
// next_ticket() below is supposed to hand out 1, 2, 3, ... but its counter
// is reborn as 0 on every call.
//
// Task: make next_ticket() count across calls — all three asserts pass.
// Constraints:
//   - the fix is one keyword
//   - no global variable, no parameter — the state stays inside the
//     function
//   - don't change the asserts

#include <cassert>

int next_ticket() {
    int counter = 0;          // a fresh object every call
    return ++counter;
}

int main() {
    assert(next_ticket() == 1);
    assert(next_ticket() == 2);
    assert(next_ticket() == 3);
    return 0;
}

// I AM NOT DONE
