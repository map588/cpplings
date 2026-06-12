// explicit1.cpp
//
// A one-argument constructor is more than a constructor — it's an
// IMPLICIT CONVERSION from the argument type. Given
//
//     struct Minutes { Minutes(int n); };
//     void wait(Minutes m);
//
// the call wait(5) compiles: the compiler quietly builds Minutes{5}.
// Convenient... until two types convert from the same thing. Then a
// swapped argument list still compiles — into the wrong meaning:
//
//     to_seconds(5, 30)   // meant 30 seconds + 5 minutes. Compiles.
//                         // Computes 5 seconds + 30 minutes.
//
// `explicit` turns the conversion off: callers must NAME the type,
// Seconds{30}, so a swap becomes a type error instead of a wrong answer.
//
// Guideline (straight from the Core Guidelines): single-argument
// constructors are `explicit` BY DEFAULT; leave one implicit only when
// the conversion is genuinely part of the type's meaning (std::string
// from a literal is the classic exception).
//
// Task: make both constructors explicit, then fix the buggy call site so
// it says what it means.

// I AM NOT DONE

#include <cassert>

struct Seconds {
    int n;
    Seconds(int n) : n(n) {}
};

struct Minutes {
    int n;
    Minutes(int n) : n(n) {}
};

int to_total_seconds(Seconds s, Minutes m) {
    return s.n + m.n * 60;
}

int main() {
    // The author meant "30 seconds plus 5 minutes" = 330.
    int total = to_total_seconds(5, 30);   // ...both ints convert. Oops.
    assert(total == 330);
    return 0;
}
