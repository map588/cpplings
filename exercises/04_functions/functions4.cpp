// functions4.cpp
//
// Overloading on the KIND of reference. (Preview of module 09 — this
// ranking is the entire mechanism that move semantics is built on.)
//
//   void f(int&)        — binds non-const lvalues only
//   void f(const int&)  — binds const lvalues... and also rvalues, as a
//                         fallback (it's the "I accept anything readable"
//                         overload — that's the const& binding rule from
//                         module 03)
//   void f(int&&)       — binds rvalues only (C++11). When present,
//                         rvalues PREFER it over const int&.
//
// And the punchline: std::move does not move anything. It is a CAST to
// rvalue reference — `static_cast<int&&>(x)` in costume. All it changes is
// WHICH OVERLOAD gets picked.
//
// Task: replace each TODO_C comparison with the letter the call really
// produces — all five asserts pass.
// Constraints:
//   - answer from the rules above first, then verify by running
//   - don't change the pick overloads or any call expression
//   - leave TODO_C itself defined as '?'
//
// Then a thought experiment (answer is in the solution's comments): if
// the int&& overload were deleted, what would pick(5) return?

#include <cassert>
#include <utility>

constexpr char TODO_C = '?';

char pick(int&)       { return 'l'; }   // mutable lvalue
char pick(const int&) { return 'c'; }   // const lvalue (or fallback)
char pick(int&&)      { return 'r'; }   // rvalue

int main() {
    int x = 1;
    const int cx = 2;

    assert(pick(x) == TODO_C);
    assert(pick(cx) == TODO_C);
    assert(pick(5) == TODO_C);
    assert(pick(x + 1) == TODO_C);          // is x + 1 an lvalue?
    assert(pick(std::move(x)) == TODO_C);   // just a cast!
    return 0;
}

// I AM NOT DONE
