// ub4.cpp — solution

#include <cassert>
#include <cstdint>
#include <cstring>

std::uint32_t parse_payload(const unsigned char* packet) {
    std::uint32_t value;
    std::memcpy(&value, packet + 1, sizeof(value));   // alignment-proof,
    return value;                                     // aliasing-proof —
}                                                     // and compiles to a
                                                      // single load anyway

int main() {
    unsigned char packet[5] = {0x7F, 0x78, 0x56, 0x34, 0x12};

    assert(parse_payload(packet) == 0x12345678);
    return 0;
}
