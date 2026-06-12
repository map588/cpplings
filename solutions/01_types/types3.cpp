// types3.cpp — solution

#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

int main() {
    std::uint8_t a = 200;
    std::uint8_t b = 100;

    auto sum = a + b;  // int arithmetic after promotion: 300, no wrap
    static_assert(std::is_same_v<decltype(sum), int>);
    assert(sum == 300);

    auto wrapped = static_cast<std::uint8_t>(a + b);  // 300 % 256
    assert(wrapped == 44);

    unsigned int zero = 0;
    assert(zero - 1 == std::numeric_limits<unsigned int>::max());
    return 0;
}
