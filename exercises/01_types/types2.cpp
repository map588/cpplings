// types2.cpp
//
// `auto` (C++11) deduces types using the same rules as template argument
// deduction. Three rules cover most of it:
//
//   1. Plain `auto` DROPS top-level const and references. You get a copy.
//   2. `auto&` keeps const (you can't bind a non-const ref to a const thing).
//   3. Arrays and functions DECAY to pointers under plain `auto`.
//
// `auto&&` is a *forwarding reference*: lvalue in → lvalue ref out,
// rvalue in → rvalue ref out. (More in the move-semantics module.)
//
// `decltype(x)` (C++11) gives the declared type of x — perfect for checking
// your understanding, since std::is_same_v (C++17 spelling) compares types
// at compile time.
//
// Each static_assert below states what the author *guessed* the deduced
// type is. Some guesses are wrong.
//
// Task: make the file compile with every static_assert intact.
//   - all four static_asserts pass at compile time
// Constraints:
//   - fix only the GUESSES (the right-hand type in each is_same_v)
//   - the declarations of a, b, c, d must not change
//   - don't delete or weaken any static_assert


#include <type_traits>

int main() {
    const int ci = 7;
    int arr[3] = {1, 2, 3};

    auto a = ci;        // copy of a const int — is the copy still const?
    auto& b = ci;       // a reference to ci — what must it preserve?
    auto c = arr;       // arrays decay...
    auto&& d = 5;       // rvalue 5 bound to a forwarding reference

    static_assert(std::is_same_v<decltype(a), const int>, "a");
    static_assert(std::is_same_v<decltype(b), int&>, "b");
    static_assert(std::is_same_v<decltype(c), int[3]>, "c");
    static_assert(std::is_same_v<decltype(d), int>, "d");

    (void)a; (void)b; (void)c; (void)d;  // silence unused-variable warnings
    return 0;
}
// I AM NOT DONE
