// move1.cpp
//
// What does moving a std::string actually DO? Steal the heap buffer:
// the destination takes over the source's allocated memory; the source
// is left "valid but unspecified" (in practice: empty).
//
// You can PROVE the theft by watching the buffer's address. If the
// string is long enough to live on the heap (long strings do; short
// ones hide inside the string object itself — SSO, module 15), then
// after a real move the destination's .data() is THE SAME POINTER the
// source used to have. A copy, by contrast, allocates fresh memory.
//
// Rules for the moved-from object: you may destroy it, assign a new
// value to it, and call anything without preconditions (.size(),
// .clear(), ...). You may NOT assume what it contains.
//
// Task: archive() copies. Make it move. (The function receives the
// string by value — its caller already decided to hand over a copy; the
// member grab inside should not pay for ANOTHER one.)

// I AM NOT DONE

#include <cassert>
#include <string>
#include <utility>

struct Archive {
    std::string contents;

    void store(std::string text) {     // by-value sink (see move5)
        contents = text;               // copies AGAIN — the buffer proves it
    }
};

int main() {
    std::string entry = "a string long enough to defeat the small-string "
                        "optimization and force a heap allocation";
    const char* heap_buffer = entry.data();

    Archive log;
    log.store(std::move(entry));       // entry's buffer moves into `text`...

    // ...and store() should have moved it on into `contents`. Same bytes,
    // same address, zero new allocations:
    assert(log.contents.data() == heap_buffer);

    // entry was plundered and that's fine — but it's still a valid object:
    entry = "reused";                  // assigning anew is always legal
    assert(entry == "reused");
    return 0;
}
