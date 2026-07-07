// templates1.cpp
//
// largest() below was written for int. Then doubles happened (notice the
// assert: 3.5 truncated through the int parameters), and now strings
// need it too.
//
// One recipe can serve all of them — a FUNCTION TEMPLATE. Here's what
// one looks like, for a different job:
//
//     template <typename T>
//     T smallest(T a, T b) { return a < b ? a : b; }
//
// The compiler DEDUCES T per call: smallest(2, 7) stamps out
// smallest<int>, smallest(s1, s2) stamps out smallest<std::string>. Each
// instantiation is a real, separate function.
//
// Deduction's one demand: CONSISTENCY. smallest(3, 2.5) deduces T=int
// from the left argument and T=double from the right — that's an error,
// not a negotiation. You break the tie yourself by naming T at the call
// site — smallest<double>(3, 2.5) — which turns deduction off and
// ordinary conversion on.
//
// Task: one largest() that serves every call in main.
//   - all four asserts pass — including the double one that today's int
//     version silently truncates
//   - exactly one definition of largest remains
//   - the mixed-type call compiles without changing its arguments
// Constraints:
//   - no overloads: one template
//   - the asserted values stay as they are (the mixed call's spelling may
//     change, its arguments 3 and 2.5 may not)
//   - don't cast the arguments

#include <cassert>
#include <string>

int largest(int a, int b) {
    return b < a ? a : b;
}

int main() {
    assert(largest(2, 7) == 7);

    assert(largest(2.5, 3.5) == 3.5);          // int version truncates!

    std::string s1 = "apple", s2 = "banana";
    assert(largest(s1, s2) == "banana");       // no int in sight

    assert(largest(3, 2.5) == 3.0);            // mixed: deduction conflict —
                                               // pick T explicitly
    return 0;
}

// I AM NOT DONE
