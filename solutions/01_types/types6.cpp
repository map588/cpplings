// types6.cpp — solution

#include <cassert>
#include <type_traits>

int main() {
    // 1. char literals are 1 byte in C++ (unlike C, where 'a' is an int).
    static_assert(sizeof('a') == 1);

    // 2. Arithmetic promotes char to int.
    auto c = 'A' + 1;
    static_assert(std::is_same_v<decltype(c), int>);
    assert(c == 66);

    // 3. Division truncates toward zero (since C++11).
    assert(-7 / 2 == -3);
    assert(-7 % 2 == -1);

    // 4. short + short promotes to int.
    short s = 1;
    auto sum = s + s;
    static_assert(std::is_same_v<decltype(sum), int>);

    // 5. Non-zero → true.
    double pi = 3.14;
    bool ok = pi;
    assert(ok == true);

    (void)sum;
    return 0;
}
