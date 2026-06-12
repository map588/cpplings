// ub6.cpp — solution

#include <cassert>
#include <cstdint>

std::uint64_t flag_mask(int bit) {
    return std::uint64_t{1} << bit;    // unsigned AND 64 bits wide:
}                                      // every bit in [0, 64) is legal

int main() {
    assert(flag_mask(0) == 1);
    assert(flag_mask(4) == 16);

    assert(flag_mask(31) == 0x80000000ull);
    assert(flag_mask(63) == 0x8000000000000000ull);
    return 0;
}
