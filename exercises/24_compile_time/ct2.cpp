// ct2.cpp
//
// The classic embedded/games/parsing trick: a LOOKUP TABLE computed at
// build time, shipped as initialized data. Recipe:
//
//     consteval std::array<uint8_t, 256> make_table() {
//         std::array<uint8_t, 256> t{};     // C++20: array ops are
//         for (...) { ... }                 // constexpr-legal
//         return t;
//     }
//     constexpr auto kTable = make_table(); // evaluated by the COMPILER
//
// consteval (module 02) is the right strength: it CANNOT run at
// runtime, so there's no risk of accidentally paying the loop on
// startup because some detail disqualified constant evaluation —
// disqualification becomes a compile error instead.
//
// The table wanted here: "hex digit value" — t['0'..'9'] = 0..9,
// t['a'..'f'] = 10..15, t['A'..'F'] = 10..15, everything else 255.
// With it, parse_hex_byte is two lookups, branchless.
//
// Task: implement make_hex_table. The static_asserts audit single
// entries; the runtime asserts use the whole pipeline.

#include <array>
#include <cassert>
#include <cstdint>

consteval std::array<std::uint8_t, 256> make_hex_table() {
    std::array<std::uint8_t, 256> t{};
    // TODO: 255 everywhere, except '0'-'9' → 0-9, 'a'-'f' and
    // 'A'-'F' → 10-15
    return t;
}

constexpr auto kHex = make_hex_table();

static_assert(kHex['7'] == 7);
static_assert(kHex['a'] == 10);
static_assert(kHex['F'] == 15);
static_assert(kHex['g'] == 255);
static_assert(kHex[' '] == 255);

// Two lookups, no branches — the table did the thinking at build time.
int parse_hex_byte(char hi, char lo) {
    if (kHex[static_cast<unsigned char>(hi)] == 255 ||
        kHex[static_cast<unsigned char>(lo)] == 255) return -1;
    return kHex[static_cast<unsigned char>(hi)] * 16 +
           kHex[static_cast<unsigned char>(lo)];
}

int main() {
    assert(parse_hex_byte('f', 'f') == 255);
    assert(parse_hex_byte('2', 'A') == 42);
    assert(parse_hex_byte('z', '1') == -1);
    return 0;
}

// I AM NOT DONE
