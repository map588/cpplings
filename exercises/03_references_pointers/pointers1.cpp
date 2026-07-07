// pointers1.cpp
//
// Unlike a reference, a pointer can be NULL — "points at nothing" is a
// legal, often useful state. That's exactly when to prefer a pointer
// parameter over a reference: when absence is part of the contract.
//
// Spell null as `nullptr` (C++11). The old spellings are integers in
// costume: NULL is a macro for 0, and 0 itself converts to any pointer
// type. That breaks overload resolution — given f(int) and f(int*),
// f(NULL) calls f(int)! nullptr has its own type, std::nullptr_t, and
// always picks the pointer overload.
//
// The flip side of nullable: every dereference of a maybe-null pointer
// needs a check. Dereferencing null is undefined behavior — typically a
// crash, but UB means the optimizer may also quietly delete your
// null-checks-after-the-fact. Run this as-is and watch the sanitizer
// report the exact line.
//
// Task: find() legitimately returns nullptr for missing keys — the caller
// forgot that. Fix the CALLER so a missing key yields 0.
//   - both asserts pass; the sanitizers stay silent; exits 0
// Constraints:
//   - find() and the table are correct — don't touch them
//   - lookup_or_zero must still go through find() and handle its nullable
//     result honestly (no pre-scanning the table, no changing find's
//     contract)

#include <cassert>

struct Entry {
    const char* key;
    int value;
};

Entry table[] = {{"alpha", 1}, {"beta", 2}, {"gamma", 3}};

// Returns a pointer to the matching entry, or nullptr if absent.
// "Absent" is part of the contract — that's WHY this returns a pointer.
Entry* find(const char* key) {
    for (Entry& e : table) {
        const char* a = e.key;
        const char* b = key;
        while (*a && *a == *b) { ++a; ++b; }
        if (*a == *b) return &e;
    }
    return nullptr;
}

int lookup_or_zero(const char* key) {
    Entry* e = find(key);
    return e->value;          // dereferenced without a check!
}

int main() {
    assert(lookup_or_zero("beta") == 2);
    assert(lookup_or_zero("delta") == 0);   // missing key → boom
    return 0;
}

// I AM NOT DONE
