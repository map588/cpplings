// concepts3.cpp — solution

#include <cassert>
#include <concepts>
#include <string>

template <typename T>
std::string describe(T) {
    return "something";
}

template <std::integral T>
std::string describe(T) {
    return "a whole number";
}

template <std::floating_point T>
std::string describe(T) {
    return "a real number";
}

int main() {
    assert(describe(42) == "a whole number");
    assert(describe(2.5) == "a real number");
    assert(describe(2.5f) == "a real number");
    assert(describe("hi") == "something");
    return 0;
}
