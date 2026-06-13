// functions1.cpp
//
// Overload resolution ranks the conversion each candidate would need:
//
//   1. exact match
//   2. promotion   — char/short/bool → int,  float → double
//   3. conversion  — int → double, long → int, double → int, ...
//
// A higher rank always wins. But if two candidates need conversions of the
// SAME rank, the call is AMBIGUOUS and does not compile — C++ refuses to
// guess. That's the bug below: long → int and long → double are both rank-3
// conversions.
//
// Each overload returns a letter so the static_asserts can show which one
// won. Task: fix the wrong guesses, then resolve the ambiguous call by
// adding a `long` overload (returning 'l').

constexpr char pick(int)    { return 'i'; }
constexpr char pick(double) { return 'd'; }

int main() {
    static_assert(pick(7) == 'i');       // exact match
    static_assert(pick(7.5) == 'i');     // TODO — exact match for which?

    // Promotions beat conversions:
    static_assert(pick('a') == 'd');     // TODO — char promotes to...
    static_assert(pick(true) == 'd');    // TODO — bool promotes to...
    static_assert(pick(7.5f) == 'i');    // TODO — float promotes to...

    // long → int and long → double are equally ranked conversions:
    static_assert(pick(7L) == 'l');      // ambiguous! add the overload
    return 0;
}

// I AM NOT DONE
