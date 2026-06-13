// ops1.cpp
//
// Operator overloading is just functions with punctuation for names:
// a + b means operator+(a, b). Two design rules carry most of the weight:
//
//   1. Binary operators that don't mutate → FREE FUNCTIONS, not members.
//      A member operator+ makes `frac + 1` work but `1 + frac` fail:
//      the left operand of a member call is never converted. A free
//      function treats both sides symmetrically.
//
//   2. Operators should mean what they mean on ints. operator== is VALUE
//      equality — and for a fraction, 1/2 IS 2/4. Compare cross-products
//      (a/b == c/d  ⇔  a*d == c*b), not raw members.
//
// The arithmetic identity worth memorizing:
//      a/b + c/d = (a*d + c*b) / (b*d)
//
// Task: both operators below are stubbed with naive member-wise bodies.
// Fix them. (The implicit Fraction(int) constructor is deliberate — it's
// what lets `half + 1` and `1 + half` work at all. explicit would break
// that; here the conversion IS the design, the module-07 exception.)

#include <cassert>

struct Fraction {
    int num;
    int den;

    Fraction(int n, int d) : num(n), den(d) {}
    Fraction(int whole) : num(whole), den(1) {}   // implicit, on purpose
};

Fraction operator+(const Fraction& a, const Fraction& b) {
    return {a.num + b.num, a.den + b.den};        // "freshman sum" — fix!
}

bool operator==(const Fraction& a, const Fraction& b) {
    return a.num == b.num && a.den == b.den;      // 1/2 != 2/4 ?!  fix!
}

int main() {
    Fraction half(1, 2);
    Fraction quarter(1, 4);

    assert(half + quarter == Fraction(3, 4));
    assert(half == Fraction(2, 4));               // value, not representation

    assert(half + 1 == Fraction(3, 2));           // rhs converts
    assert(1 + half == Fraction(3, 2));           // lhs converts — free
                                                  // function makes this work
    return 0;
}

// I AM NOT DONE
