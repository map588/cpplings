// variadic1.cpp — solution

#include <cassert>
#include <cstddef>

template <typename... Ts>
auto sum(Ts... args) {
    return (0 + ... + args);             // binary fold: empty pack → 0
}

template <typename... Ts>
constexpr std::size_t count_args(Ts...) {
    return sizeof...(Ts);
}

template <typename... Ts>
bool all_positive(Ts... args) {
    return ((args > 0) && ...);          // empty pack → true, by the standard
}

int main() {
    assert(sum(1, 2, 3, 4) == 10);
    assert(sum(0.5, 2) == 2.5);
    assert(sum() == 0);

    static_assert(count_args(9, 9, 9) == 3);
    static_assert(count_args() == 0);

    assert(all_positive(1, 2, 3));
    assert(!all_positive(1, -2, 3));
    assert(all_positive());
    return 0;
}
