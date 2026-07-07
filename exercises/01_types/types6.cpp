// types6.cpp  — QUIZ for module 01
//
// Everything here was covered in types1–types5, plus two classic trivia
// items called out in comments.
//
// Task: fix each line marked TODO so all checks pass. Predict each
// answer BEFORE running.
//   - compiles; every static_assert and assert passes; exits 0
// Constraints:
//   - change only the guessed values/types on the TODO lines — never the
//     expression being tested, and don't delete a check you can't answer


#include <cassert>
#include <type_traits>

int main() {
    // 1. In C, 'a' is an int. In C++ it's a char — one of the small
    //    C/C++ divergences. So what does sizeof say here?
    static_assert(sizeof('a') == 0);  // TODO

    // 2. But what type does ARITHMETIC on a char produce? (types3 knows.)
    auto c = 'A' + 1;
    static_assert(std::is_same_v<decltype(c), char>);  // TODO: promoted to what?
    assert(c == 0);                                    // TODO: 'A' is 65

    // 3. Integer division truncates TOWARD ZERO (guaranteed since C++11;
    //    before that, rounding direction was implementation-defined!).
    //    The identity (a/b)*b + a%b == a always holds.
    assert(-7 / 2 == 0);   // TODO
    assert(-7 % 2 == 0);   // TODO

    // 4. short + short is not a short.
    short s = 1;
    auto sum = s + s;
    static_assert(std::is_same_v<decltype(sum), short>);  // TODO

    // 5. What survives when a floating-point number is converted to bool?
    //    (It's the same conversion that makes `if (ptr)` work.)
    double pi = 3.14;
    bool ok = pi;  // (brace init would reject this narrowing!)
    assert(ok == false);  // TODO

    (void)sum;
    return 0;
}
// I AM NOT DONE
