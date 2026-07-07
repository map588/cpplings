// valcat1.cpp
//
// The probe for this whole module: decltype with DOUBLE PARENTHESES
// reports an expression's value category as a reference flavor:
//
//     decltype((expr))  →  T&    the expression is an LVALUE
//                          T&&   ...an XVALUE
//                          T     ...a PRVALUE
//
// (Single parens, decltype(name), reports the declared type of the
// name — a different question. The inner parens force "treat it as an
// expression".)
//
// How to classify without a cheat sheet:
//   - does the expression refer to an object that persists beyond the
//     expression — something you could sensibly take the address of?
//     LVALUE.
//   - does it conjure a fresh, homeless value? PRVALUE.
//   - is it a persistent object that's been marked "expiring — may be
//     plundered"? XVALUE.
// Overload resolution (04), reference binding (03) and movability (09)
// all key off this taxonomy — these seven expressions are the classics
// to internalize.
//
// Task: some of the category claims below are wrong. Fix them.
//   - every static_assert passes
//   - classify each expression from the rules above FIRST; only then
//     let the compiler check you
// Constraints:
//   - edit only the claimed type (int / int& / int&&) in each assert —
//     never the expression being probed
//   - do not delete or reorder the static_asserts

#include <type_traits>
#include <utility>

// decltype never EVALUATES its operand — no increment actually runs
// below (a lesson in itself: unevaluated contexts, like sizeof and
// noexcept). clang politely warns about the side effect that won't
// happen; here that's exactly the point:
#ifdef __clang__
#pragma clang diagnostic ignored "-Wunevaluated-expression"
#endif

int main() {
    int x = 0;
    int* p = &x;
    int arr[3] = {};

    // Names and things with addresses — which reference flavor?
    static_assert(std::is_same_v<decltype((x)), int>, "a name");
    static_assert(std::is_same_v<decltype((*p)), int>, "a dereference");
    static_assert(std::is_same_v<decltype((arr[1])), int>, "an element");

    // Computed values:
    static_assert(std::is_same_v<decltype((x + 1)), int&>, "a sum");

    // The increment twins — one hands back THE OBJECT, one a copy:
    static_assert(std::is_same_v<decltype((++x)), int>, "pre-increment");
    static_assert(std::is_same_v<decltype((x++)), int&>, "post-increment");

    // The cast that started module 09:
    static_assert(std::is_same_v<decltype((std::move(x))), int&>, "move");

    (void)x; (void)p; (void)arr;
    return 0;
}

// I AM NOT DONE
