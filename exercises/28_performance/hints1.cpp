// hints1.cpp
//
// You can TELL the optimizer which way a branch usually goes:
//
//     if (err) [[unlikely]] { ... }        // C++20 attributes
//     if (ok)  [[likely]]   { ... }
//     if (__builtin_expect(err, 0)) { }    // the pre-C++20 spelling
//                                          // (this is what Linux's
//                                          // likely()/unlikely() are)
//
// The compiler lays out the expected path fall-through-straight so
// the CPU's front end stays fed, and parks the cold path out of the
// way. Two rules keep you honest:
//
//   1. Hints NEVER change what the program computes — only how the
//      machine code is arranged. A wrong hint costs speed, not
//      correctness.
//   2. Hint only what you've measured. Programmers guess branch
//      probabilities about as well as they guess bottlenecks.
//
// Then there is the hint with TEETH:
//
//     __builtin_unreachable()              // (std::unreachable in C++23)
//
// This is not a check — it is a PROMISE: "control never gets here",
// and the optimizer deletes every path that would. If control DOES
// get here, that's undefined behavior — module 22's contract, worst
// edition, because the optimizer has been actively building on the
// lie. The difference matters:
//
//     assert(cond)             — CHECKS at runtime (until NDEBUG)
//     __builtin_unreachable()  — promises, checks NOTHING, ever
//
// UBSan instruments it (that's why this exercise keeps sanitizers
// on): reaching one reports "execution reached an unreachable
// program point" and dies. Run this file and watch it happen.
//
// parse_digit below promised its author that only digits arrive.
// Then someone fed it user input. For UNTRUSTED data the function
// must handle the bad case — the promise was never yours to make.
//
// Task: make parse_digit total — return -1 for a non-digit — and
// make the caller skip those.
//   - every assert passes, UBSan stays silent
// Constraints:
//   - keep the [[likely]]/[[unlikely]] annotations that are already
//     correct; digits ARE the common case in this input
//   - don't validate before calling — the fix belongs INSIDE
//     parse_digit (one total function beats N careful callers)

#include <cassert>

// Digits dominate real input, so the happy path carries the hint.
int parse_digit(char c) {
    if (c >= '0' && c <= '9') [[likely]] {
        return c - '0';
    }
    __builtin_unreachable();   // "only digits arrive" — says who?
}

int main() {
    // Trusted input: fine either way.
    assert(parse_digit('7') == 7);
    assert(parse_digit('0') == 0);

    // Untrusted input: sum the digits, skip the junk.
    const char* input = "4x7q2";
    int sum = 0;
    for (const char* p = input; *p != '\0'; ++p) {
        int d = parse_digit(*p);
        if (d < 0) [[unlikely]] {
            continue;                     // junk is the rare case
        }
        sum += d;
    }
    assert(sum == 13);
    return 0;
}

// I AM NOT DONE
