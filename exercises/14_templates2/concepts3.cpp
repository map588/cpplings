// concepts3.cpp
//
// Before concepts, "different code per category of T" meant enable_if
// gymnastics or tag dispatch. Now: write several templates with the
// SAME signature but different constraints, and overload resolution
// picks the MOST CONSTRAINED one the argument satisfies (subsumption).
// A constrained template beats an unconstrained one outright — no
// ambiguity, no tie-breaking ritual.
//
// Two spellings for "T, which must satisfy SomeConcept", both worth
// recognizing on sight:
//
//     template <SomeConcept T> ReturnType f(T x);   // constrained param
//     ReturnType f(SomeConcept auto x);             // C++20 abbreviated
//
// The ready-made category concepts you met in concepts1 slot straight
// in there.
//
// Task: add overloads of describe() so that every assert passes.
//   - integer arguments report "a whole number"
//   - floating-point arguments report "a real number"
//   - everything else still falls through to "something"
// Constraints:
//   - keep the unconstrained fallback exactly as it is
//   - no enable_if, no tag dispatch, no if constexpr, no
//     specializations — constraints alone must steer each call
//   - do not change the asserts

#include <cassert>
#include <concepts>
#include <string>

template <typename T>
std::string describe(T) {
    return "something";
}

// TODO: an overload for integral types returning "a whole number"
// TODO: an overload for floating-point types returning "a real number"

int main() {
    assert(describe(42) == "a whole number");
    assert(describe(2.5) == "a real number");
    assert(describe(2.5f) == "a real number");
    assert(describe("hi") == "something");
    return 0;
}

// I AM NOT DONE
