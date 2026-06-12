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
// The classics to internalize, because overload resolution (04),
// binding (03) and movability (09) all key off them:
//
//   named variable        lvalue   (even if its TYPE is int&&!)
//   x + 1, 42, x++        prvalue
//   ++x, *p, arr[i]       lvalue   (++x and x++ disagree!)
//   std::move(x)          xvalue
//
// Task: fix the wrong category claims.

// I AM NOT DONE

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
