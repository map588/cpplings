// concepts3.cpp
//
// Before concepts, "different code per category of T" meant enable_if
// gymnastics or tag dispatch. Now: write several templates with
// different constraints, and overload resolution picks the MOST
// CONSTRAINED one that matches (subsumption):
//
//     template <typename T>            std::string describe(T);  // anything
//     template <std::integral T>       std::string describe(T);  // tighter
//     template <std::floating_point T> std::string describe(T);  // tighter
//
//     describe(42)      → the integral version (more constrained beats
//                         unconstrained — no ambiguity)
//     describe(2.5)     → the floating_point version
//     describe("hi")    → the fallback
//
// The `template <std::integral T>` shorthand means "T, which must
// satisfy std::integral". Even shorter — the C++20 abbreviated form:
//
//     std::string describe(std::integral auto x);
//
// Task: only the fallback exists. Add the two constrained overloads the
// asserts demand.

// I AM NOT DONE

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
