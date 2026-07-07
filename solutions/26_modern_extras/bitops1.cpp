// bitops1.cpp — solution
// countr_zero names the lowest set bit; x &= x - 1 clears it. Together
// they visit exactly the set bits, lowest first.

#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>

// ---- warm-ups: a peripheral's mask constants, audited at compile time ----

constexpr std::uint32_t UART_TX_DONE = 0x0000'0800;   // a single flag
constexpr std::uint32_t ENABLED      = 0b1011'0110;   // channel enable mask

static_assert(std::has_single_bit(UART_TX_DONE));     // valid one-flag mask
static_assert(!std::has_single_bit(ENABLED));         // five flags != a flag

static_assert(std::popcount(ENABLED) == 5);           // channels enabled
static_assert(std::bit_width(ENABLED) == 8);          // highest is bit 7
static_assert(std::countr_zero(ENABLED) == 1);        // lowest set bit

// Rotate an 8-bit value: the top bit falls off... and comes back around.
static_assert(std::rotl(std::uint8_t{0b1000'0001}, 1) == 0b0000'0011);

// ---- the IRQ dispatch idiom -----------------------------------------------

// Record the index of every set bit in `pending`, LOWEST first, into
// fired[]; return the number recorded.
std::size_t dispatch_all(std::uint32_t pending, std::array<int, 8>& fired) {
    std::size_t n = 0;
    while (pending != 0) {
        assert(n < fired.size() && "more pending IRQs than fired[] holds");
        fired[n++] = std::countr_zero(pending);   // which IRQ fired
        pending &= pending - 1;                   // clear the lowest set bit
    }
    return n;
}

int main() {
    constexpr std::uint32_t pending =
        (1u << 2) | (1u << 3) | (1u << 7) | (1u << 19) | (1u << 31);

    std::array<int, 8> fired{};
    std::size_t n = dispatch_all(pending, fired);

    assert(n == 5);
    assert(fired[0] == 2);
    assert(fired[1] == 3);
    assert(fired[2] == 7);
    assert(fired[3] == 19);
    assert(fired[4] == 31);   // the sign bit, were this signed — it isn't

    assert(dispatch_all(0, fired) == 0);   // quiet controller: no work
    return 0;
}

// Note: <bit> also holds std::bit_cast (C++20 — module 22) and, in
// C++23, std::byteswap for endian flips.
