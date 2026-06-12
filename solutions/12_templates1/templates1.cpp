// templates1.cpp — solution

#include <cassert>
#include <string>

template <typename T>
T largest(T a, T b) {
    return b < a ? a : b;
}

int main() {
    assert(largest(2, 7) == 7);                // largest<int>

    assert(largest(2.5, 3.5) == 3.5);          // largest<double>

    std::string s1 = "apple", s2 = "banana";
    assert(largest(s1, s2) == "banana");       // largest<std::string>

    assert(largest<double>(3, 2.5) == 3.0);    // explicit: 3 converts
    return 0;
}
