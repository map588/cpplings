// part1_quiz.cpp  — QUIZ for Part I (modules 01–05)
//
// Part I is done. Before classes arrive, check that the foundations
// hold: a few probes per module, every one a fact you have already
// met. (Module 06's lessons need two files to show at all — linkage1–5
// and cabi1 were their own quiz.)
//
// One new tool appears below: std::is_assignable_v<A, B> answers, at
// compile time, "would `a = b` compile for an `a` of type A and a `b`
// of type B?" It lets a static_assert ask what const forbids.
//
// Task: replace every TODO, TODO_BOOL and TODO_C with your prediction.
//   - the program compiles and every assert and static_assert passes
// Constraints:
//   - change only the TODO values — the probes stay as they are
//   - predict on paper first: a value copied out of an error message
//     graduates the compiler, not you

#include <cassert>
#include <type_traits>
#include <utility>

constexpr int TODO = -1;
constexpr bool TODO_BOOL = false;
constexpr char TODO_C = '?';

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
    assert(whole == TODO);

    // Module 01 — integral promotion: is char + char an int?
    char a = 'a', b = 'b';
    static_assert(std::is_same_v<decltype(a + b), int> == TODO_BOOL);
    (void)a; (void)b;

    // Module 01 — unsigned arithmetic wraps. Is 0u - 1 greater than 0?
    unsigned zero = 0;
    assert((zero - 1 > 0) == TODO_BOOL);

    // Module 01 — plain auto makes a copy; auto& makes an alias. Which
    // of the two is still const?
    const int limit = 10;
    auto copy = limit;
    auto& alias = limit;
    static_assert(std::is_const_v<decltype(copy)> == TODO_BOOL);
    static_assert(
        std::is_const_v<std::remove_reference_t<decltype(alias)>> == TODO_BOOL);
    (void)copy; (void)alias;

    // Module 02 — read each pointer type right to left, then say what
    // const forbids.
    using View  = const int*;     // pointer to const int
    using Fixed = int* const;     // const pointer to int
    static_assert(std::is_assignable_v<View&, int*> == TODO_BOOL);    // re-aim a View?
    static_assert(std::is_assignable_v<Fixed&, int*> == TODO_BOOL);   // re-aim a Fixed?
    static_assert(                                                    // write through a View?
        std::is_assignable_v<decltype(*std::declval<View>()), int> == TODO_BOOL);
    static_assert(                                                    // write through a Fixed?
        std::is_assignable_v<decltype(*std::declval<Fixed>()), int> == TODO_BOOL);

    // Module 03 — what does assigning to a reference do?
    int x = 1, y = 2;
    int& rx = x;
    rx = y;
    y = 50;
    assert(x == TODO);
    assert(rx == TODO);
    assert((&rx == &x) == TODO_BOOL);     // is rx still an alias for x?

    // Module 04 — exact match, then promotion, then conversion:
    static_assert(rank('x') == TODO_C);
    static_assert(rank(2.5f) == TODO_C);
    static_assert(rank(true) == TODO_C);

    // Module 04 — reference overloads, the mechanism under module 09:
    int n = 1;
    const int cn = 2;
    assert(kind(n) == TODO_C);
    assert(kind(cn) == TODO_C);
    assert(kind(n + 1) == TODO_C);
    assert(kind(std::move(n)) == TODO_C);

    // Module 04 — one static local, one default argument, three calls:
    assert(tally() == TODO);
    assert(tally(1) == TODO);
    assert(tally() == TODO);

    // Module 05 — which version does the parent namespace hand out?
    static_assert(lib::version() == TODO);
    static_assert(lib::v1::version() == TODO);
    return 0;
}

// I AM NOT DONE
