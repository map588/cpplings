// types2.cpp — solution

#include <type_traits>

int main() {
    const int ci = 7;
    int arr[3] = {1, 2, 3};

    auto a = ci;        // plain auto drops top-level const → int
    auto& b = ci;       // reference must preserve const → const int&
    auto c = arr;       // array decays to pointer → int*
    auto&& d = 5;       // rvalue bound to forwarding ref → int&&

    static_assert(std::is_same_v<decltype(a), int>, "a");
    static_assert(std::is_same_v<decltype(b), const int&>, "b");
    static_assert(std::is_same_v<decltype(c), int*>, "c");
    static_assert(std::is_same_v<decltype(d), int&&>, "d");

    (void)a; (void)b; (void)c; (void)d;
    return 0;
}
