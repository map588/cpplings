// functions5.cpp — solution

#include <cassert>

int bump(int by = 2) {
    static int total = 0;
    return total += by;
}

constexpr char which(int)    { return 'i'; }
constexpr char which(double) { return 'd'; }

int main() {
    // 1. total accumulates: 2, then +3 → 5, then +2 → 7.
    assert(bump() == 2);
    assert(bump(3) == 5);
    assert(bump() == 7);

    // 2. short → int and float → double are promotions.
    static_assert(which(short{4}) == 'i');
    static_assert(which(0.5f) == 'd');

    // 3. The cast picks a rank-1 exact match and ends the ambiguity.
    unsigned u = 4u;
    char c = which(static_cast<int>(u));
    assert(c == 'i' || c == 'd');
    return 0;
}
