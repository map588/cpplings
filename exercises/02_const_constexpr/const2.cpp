// const2.cpp
//
// With pointers, const can protect two different things: the POINTEE or the
// POINTER. Read the declaration right-to-left:
//
//   const int* p;        // p is a pointer to (an) int (that is) const
//                        //   → *p = 1; ERROR     p = &other; OK
//   int* const p;        // p is a const pointer to int
//                        //   → *p = 1; OK        p = &other; ERROR
//   const int* const p;  // const pointer to const int — neither changes
//
// ("east const" style writes `int const* p` — identical meaning; the rule
// is that const binds to what's on its LEFT, unless it's leftmost.)
//
// Also: a pointer-to-const can point at a non-const object — it's a
// read-only VIEW, it doesn't make the object immutable. The reverse
// (plain pointer at a const object) is forbidden: it would launder away
// the promise.
//
// The three declarations below don't match how the pointers are used.
//
// Task: fix ONLY the three declarations (move/add/remove const) so the
// program compiles and passes.
//   - compiles; all asserts pass; exits 0
// Constraints:
//   - the usage lines are correct and must not change
//   - each declaration must also FORBID what its comment says never
//     happens (writing through viewer, re-aiming fixed, either through
//     frozen). Three plain `int*` would compile — and miss the point.

#include <cassert>

int main() {
    int score = 10;
    int backup = 99;

    // "viewer" only ever READS through the pointer, but must be able to
    // move between targets.
    int* const viewer = &score;
    viewer = &backup;          // re-aim: must work
    assert(*viewer == 99);     // read: must work

    // "fixed" stays aimed at `score` forever, but WRITES through it.
    const int* fixed = &score;
    *fixed = 20;               // write: must work
    assert(score == 20);

    // "frozen" neither re-aims nor writes.
    int* frozen = &score;      // should promise both kinds of const
    assert(*frozen == 20);

    return 0;
}

// I AM NOT DONE
