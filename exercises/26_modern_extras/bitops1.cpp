// bitops1.cpp
//
// <bit> (C++20): the bit-twiddling toolbox.
//
// An interrupt controller hands you one 32-bit PENDING register: bit i
// set means IRQ i fired. For decades the code that walks such registers
// was hand-rolled loops or compiler intrinsics (__builtin_ctz,
// _mm_popcnt_u32...). <bit> makes the toolbox standard, portable, and
// constexpr — the compiler still emits the single CPU instruction:
//
//   std::has_single_bit(x)  exactly one bit set — "is this a valid
//                           single-flag mask?" (== is-power-of-two)
//   std::popcount(x)        how many bits set — channels enabled
//   std::countr_zero(x)     count of trailing zeros == the INDEX of the
//                           lowest set bit: WHICH interrupt fired
//   std::bit_width(x)       1 + index of the highest set bit — how many
//                           bits x occupies
//   std::rotl / std::rotr   rotate: bits that fall off one end come
//                           back on the other (hash/CRC mixing, without
//                           the UB edge cases of shifting)
//
// The classic dispatch loop, lowest IRQ first:
//
//   while pending != 0:  i = countr_zero(pending);  handle(i);
//   pending &= pending - 1;   // clears the LOWEST set bit: subtracting
//                             // 1 flips that bit and the zeros below
//                             // it, and & wipes exactly those
//
// All <bit> functions take UNSIGNED integer types only — by design;
// signed sign bits and these operations don't mix.
//
// Task: replace every TODO so the file compiles (the static_asserts run
// at compile time) and then runs to exit 0.
//   - warm-ups: predict each constant,
//   - dispatch_all(): record the index of every set bit in `pending`,
//     lowest first, into fired[]; return how many there were.
// Constraints:
//   - dispatch_all must iterate over SET BITS only — no 0..31 scan that
//     tests every position — and must not hardcode the test masks,
//   - don't change the masks, main(), or any (static_)assert.

#include <array>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>

constexpr int TODO = -1;

// ---- warm-ups: a peripheral's mask constants, audited at compile time ----

constexpr std::uint32_t UART_TX_DONE = 0x0000'0800;   // a single flag
constexpr std::uint32_t ENABLED      = 0b1011'0110;   // channel enable mask

static_assert(std::has_single_bit(UART_TX_DONE));     // valid one-flag mask
static_assert(!std::has_single_bit(ENABLED));         // five flags != a flag

static_assert(std::popcount(ENABLED) == TODO);        // channels enabled?
static_assert(std::bit_width(ENABLED) == TODO);       // highest bit + 1?
static_assert(std::countr_zero(ENABLED) == TODO);     // lowest set bit?

// Rotate an 8-bit value: the top bit falls off... and comes back around.
static_assert(std::rotl(std::uint8_t{0b1000'0001}, 1) == TODO);

// ---- the IRQ dispatch idiom -----------------------------------------------

// Record the index of every set bit in `pending`, LOWEST first, into
// fired[]; return the number recorded.
std::size_t dispatch_all(std::uint32_t pending, std::array<int, 8>& fired) {
    (void)pending;
    (void)fired;
    return 0;                                          // TODO
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

// I AM NOT DONE
