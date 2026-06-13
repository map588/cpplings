// types6.cpp  — QUIZ for module 01
//
// Everything here was covered in types1–types5, plus two classic trivia
// items called out in comments. Fix each line marked TODO so all checks
// pass. Predict each answer BEFORE running.


#include <cassert>
#include <type_traits>

int main() {
    // 1. In C++, character literals are of type char, so sizeof('a') is 1.
    //    (In C, 'a' is an int! One of the small C/C++ divergences.)
    static_assert(sizeof('a') == 0);  // TODO

    // 2. But ARITHMETIC on char promotes to int first:
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

    // 5. Converting any non-zero number to bool gives true — this is why
    //    `if (ptr)` works, and also why `bool ok = pi;` is true.
    double pi = 3.14;
    bool ok = pi;  // (brace init would reject this narrowing!)
    assert(ok == false);  // TODO

    (void)sum;
    return 0;
}
// I AM NOT DONE
