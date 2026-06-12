// valcat1.cpp — solution

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

    // lvalues → T&
    static_assert(std::is_same_v<decltype((x)), int&>, "a name");
    static_assert(std::is_same_v<decltype((*p)), int&>, "a dereference");
    static_assert(std::is_same_v<decltype((arr[1])), int&>, "an element");

    // prvalue → T
    static_assert(std::is_same_v<decltype((x + 1)), int>, "a sum");

    // ++x returns the object itself (lvalue); x++ returns a copy of the
    // old value (prvalue):
    static_assert(std::is_same_v<decltype((++x)), int&>, "pre-increment");
    static_assert(std::is_same_v<decltype((x++)), int>, "post-increment");

    // xvalue → T&&
    static_assert(std::is_same_v<decltype((std::move(x))), int&&>, "move");

    (void)x; (void)p; (void)arr;
    return 0;
}
