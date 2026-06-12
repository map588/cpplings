// ct2.cpp — solution

#include <array>
#include <cassert>
#include <cstdint>

consteval std::array<std::uint8_t, 256> make_hex_table() {
    std::array<std::uint8_t, 256> t{};
    for (auto& e : t) e = 255;
    for (char c = '0'; c <= '9'; ++c) t[c] = static_cast<std::uint8_t>(c - '0');
    for (char c = 'a'; c <= 'f'; ++c) t[c] = static_cast<std::uint8_t>(c - 'a' + 10);
    for (char c = 'A'; c <= 'F'; ++c) t[c] = static_cast<std::uint8_t>(c - 'A' + 10);
    return t;
}

constexpr auto kHex = make_hex_table();

static_assert(kHex['7'] == 7);
static_assert(kHex['a'] == 10);
static_assert(kHex['F'] == 15);
static_assert(kHex['g'] == 255);
static_assert(kHex[' '] == 255);

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
