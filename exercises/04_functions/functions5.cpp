// functions5.cpp  — QUIZ for module 04
//
// Defaults, static locals, and overload ranking, together. Predict
// everything BEFORE running.

#include <cassert>

constexpr int TODO = -1;

// A default argument AND a static local in one function:
int bump(int by = 2) {
    static int total = 0;
    return total += by;
}

constexpr char which(int)    { return 'i'; }
constexpr char which(double) { return 'd'; }

int main() {
    // 1. The static persists across calls; the default applies per call.
    assert(bump() == TODO);
    assert(bump(3) == TODO);
    assert(bump() == TODO);

    // 2. Promotions (not conversions) — rank these:
    static_assert(which(short{4}) == '?');   // TODO
    static_assert(which(0.5f) == '?');       // TODO

    // 3. unsigned → int and unsigned → double are BOTH conversions, so
    //    this call is ambiguous and won't compile. Fix the call site —
    //    say which one you mean with a static_cast (keep the 4u!).
    unsigned u = 4u;
    char c = which(u);
    assert(c == 'i' || c == 'd');  // either choice is fine — just choose
    return 0;
}

// I AM NOT DONE
