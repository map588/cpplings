// layout4.cpp
//
// An empty class has sizeof == 1. Not 0 — every OBJECT must have a
// unique address, so even nothing takes a byte. The trouble starts when
// the nothing becomes a MEMBER:
//
//     struct Set {
//         Compare cmp;       // empty: 1 byte... + 7 PADDING before ptr
//         Node* root;        // 8
//     };                     // sizeof == 16. The comparator: 8 bytes.
//
// Classically the cure was inheritance (the "empty BASE optimization" —
// bases may occupy zero bytes), which contorted designs. C++20 made it
// an attribute:
//
//     struct Set {
//         [[no_unique_address]] Compare cmp;   // may truly take 0 bytes
//         Node* root;
//     };                                       // sizeof == 8
//
// This is precisely how unique_ptr stores your stateless deleter for
// free (deleter1's sizeof asserts!) — and when the comparator/deleter
// HAS state, the attribute gracefully does nothing and the member gets
// its bytes back.
//
// Task: fix Timeline's size with the attribute, then settle the quiz.

// I AM NOT DONE

#include <cstddef>

constexpr std::size_t TODO = 9999;

struct ByStart {                      // a comparator with no state
    bool operator()(int a, int b) const { return a < b; }
};

struct Timeline {
    ByStart cmp;                      // costs a word, carries nothing
    double* events;
    std::size_t count;
};

static_assert(sizeof(Timeline) == 16,
              "the empty comparator should cost nothing");

// Quiz:
struct Empty {};
struct Holder { Empty e; char c; };

static_assert(sizeof(Empty) == TODO);     // nothing still needs an address
static_assert(sizeof(Holder) == TODO);    // two objects, two addresses

int main() { return 0; }
