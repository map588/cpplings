// templates6.cpp — solution

#include <cassert>
#include <cstddef>

// Reference-to-array: no decay; N deduced at compile time.
template <typename T, std::size_t N>
constexpr std::size_t count(T (&)[N]) {
    return N;
}

int main() {
    int readings[] = {62, 58, 71, 64};

    assert(count(readings) == 4);

    static_assert(count(readings) == 4);
    int copy[count(readings)] = {};
    assert(sizeof(copy) == sizeof(readings));
    return 0;
}
