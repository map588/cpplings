// templates1.cpp
//
// largest() below was written for int. Then doubles happened (notice the
// assert: 3.5 truncated through the int parameters), and now strings
// need it too.
//
// One recipe serves all of them:
//
//     template <typename T>
//     T largest(T a, T b) { return b < a ? a : b; }
//
// The compiler DEDUCES T per call: largest(2, 7) stamps out largest<int>,
// largest(s1, s2) stamps out largest<std::string>. Each instantiation is
// a real, separate function.
//
// Deduction's one demand: CONSISTENCY. largest(3, 2.5) deduces T=int
// from the left argument and T=double from the right — that's an error,
// not a negotiation. You break the tie yourself: largest<double>(3, 2.5)
// turns deduction off and conversion on.
//
// Task: templatize largest, then fix the mixed-type call at the bottom
// with an explicit template argument.

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
