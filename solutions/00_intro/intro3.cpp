// intro3.cpp — solution

#include <cassert>

constexpr int square(int x) {
    return x * x;
}

int main() {
    static_assert(square(3) == 9, "square(3) should be 9");

    int five = 5;
    assert(square(five) == 25);
    return 0;
}
