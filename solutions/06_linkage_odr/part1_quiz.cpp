// part1_quiz.cpp — solution

#include <cassert>
#include <type_traits>
#include <utility>

// ---- module 04: overload ranking -------------------------------------
constexpr char rank(int)    { return 'i'; }
constexpr char rank(double) { return 'd'; }

char kind(int&)       { return 'l'; }
char kind(const int&) { return 'c'; }
char kind(int&&)      { return 'r'; }

// ---- module 04: a static local and a default argument ----------------
int tally(int step = 5) {
    static int total = 0;
    return total += step;
}

// ---- module 05: an inline namespace -----------------------------------
namespace lib {
    namespace v1        { constexpr int version() { return 1; } }
    inline namespace v2 { constexpr int version() { return 2; } }
}

int main() {
    // Module 01 — old-style init narrows without a word:
    double ratio = 7.9;
    int whole = ratio;
    assert(whole == 7);

    // Module 01 — integral promotion: is char + char an int?
    char a = 'a', b = 'b';
    static_assert(std::is_same_v<decltype(a + b), int> == true);
    (void)a; (void)b;

    // Module 01 — unsigned arithmetic wraps. Is 0u - 1 greater than 0?
    unsigned zero = 0;
    assert((zero - 1 > 0) == true);

    // Module 01 — plain auto makes a copy; auto& makes an alias. Which
    // of the two is still const?
    const int limit = 10;
    auto copy = limit;
    auto& alias = limit;
    static_assert(std::is_const_v<decltype(copy)> == false);
    static_assert(
        std::is_const_v<std::remove_reference_t<decltype(alias)>> == true);
    (void)copy; (void)alias;

    // Module 02 — read each pointer type right to left, then say what
    // const forbids.
    using View  = const int*;     // pointer to const int
    using Fixed = int* const;     // const pointer to int
    static_assert(std::is_assignable_v<View&, int*> == true);    // re-aim a View?
    static_assert(std::is_assignable_v<Fixed&, int*> == false);   // re-aim a Fixed?
    static_assert(                                                    // write through a View?
        std::is_assignable_v<decltype(*std::declval<View>()), int> == false);
    static_assert(                                                    // write through a Fixed?
        std::is_assignable_v<decltype(*std::declval<Fixed>()), int> == true);

    // Module 03 — what does assigning to a reference do?
    int x = 1, y = 2;
    int& rx = x;
    rx = y;
    y = 50;
    assert(x == 2);
    assert(rx == 2);
    assert((&rx == &x) == true);     // is rx still an alias for x?

    // Module 04 — exact match, then promotion, then conversion:
    static_assert(rank('x') == 'i');
    static_assert(rank(2.5f) == 'd');
    static_assert(rank(true) == 'i');

    // Module 04 — reference overloads, the mechanism under module 09:
    int n = 1;
    const int cn = 2;
    assert(kind(n) == 'l');
    assert(kind(cn) == 'c');
    assert(kind(n + 1) == 'r');
    assert(kind(std::move(n)) == 'r');

    // Module 04 — one static local, one default argument, three calls:
    assert(tally() == 5);
    assert(tally(1) == 6);
    assert(tally() == 11);

    // Module 05 — which version does the parent namespace hand out?
    static_assert(lib::version() == 2);
    static_assert(lib::v1::version() == 1);
    return 0;
}
