// ops1.cpp — solution

#include <cassert>

struct Fraction {
    int num;
    int den;

    Fraction(int n, int d) : num(n), den(d) {}
    Fraction(int whole) : num(whole), den(1) {}
};

Fraction operator+(const Fraction& a, const Fraction& b) {
    return {a.num * b.den + b.num * a.den, a.den * b.den};
}

bool operator==(const Fraction& a, const Fraction& b) {
    return a.num * b.den == b.num * a.den;   // cross-multiply: value equality
}

int main() {
    Fraction half(1, 2);
    Fraction quarter(1, 4);

    assert(half + quarter == Fraction(3, 4));
    assert(half == Fraction(2, 4));

    assert(half + 1 == Fraction(3, 2));
    assert(1 + half == Fraction(3, 2));
    return 0;
}
