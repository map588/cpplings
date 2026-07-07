// endian1.cpp
//
// A std::uint32_t is four bytes — in WHICH ORDER? That's the host's
// ENDIANNESS. x86 and (virtually every) ARM Linux are little-endian:
// least significant byte at the lowest address, so 0x12345678 sits in
// memory as 78 56 34 12. Wire and network protocols traditionally send
// the MOST significant byte first ("big-endian", a.k.a. network order):
// 12 34 56 78. std::endian (C++20, <bit>) names the host's order as a
// compile-time fact.
//
// So memcpy-ing an integer into a packet is NOT UB — it's module 22's
// blessed way to move bytes (ASan/UBSan stay green through this whole
// file) — but it copies the bytes in HOST order: the wire format then
// changes with the machine. The portable idiom is shift-and-mask
// ARITHMETIC: arithmetic is defined on VALUES, not on stored bytes.
// (x >> 24) means "the top 8 bits of the value" on every CPU ever
// made, so building and splitting bytes by shifting produces identical
// wire bytes on any host — no endianness test, no branch.
//
// The wire format under repair (a sensor report):
//
//     byte 0..3   device_id   u32, big-endian
//     byte 4..5   reading     u16, big-endian
//
// Task: make serialize()/deserialize() produce and consume exactly this
// format on ANY host.
//   - every byte-level, round-trip, and captured-packet assert passes
// Constraints:
//   - don't change main() or any assert
//   - derive each wire byte from the integer's VALUE — no memcpy or
//     pointer tricks between the integers and the buffer
//   - no branching on std::endian: the same code must be correct on
//     either host order
//   - the static_assert in main() pins the GRADING host to little-endian
//     — this course targets little-endian machines, and that's exactly
//     what makes the shipped memcpy bug visible. Leave it in place;
//     nothing in serialize()/deserialize() may rely on it.
//
// (Note: std::byteswap arrives in C++23 — handy when you must flip a
// whole integer; shift-and-mask never needs the flip in the first
// place.)

#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>

struct SensorPacket {
    std::uint32_t device_id;
    std::uint16_t reading;
};

// TODO: emit big-endian bytes regardless of the host's order.
void serialize(const SensorPacket& p, std::uint8_t out[6]) {
    std::memcpy(out,     &p.device_id, 4);   // host order — whatever that is
    std::memcpy(out + 4, &p.reading,   2);
}

// TODO: same contract on the way back in.
SensorPacket deserialize(const std::uint8_t in[6]) {
    SensorPacket p{};
    std::memcpy(&p.device_id, in,     4);
    std::memcpy(&p.reading,   in + 4, 2);
    return p;
}

int main() {
    static_assert(std::endian::native == std::endian::little,
                  "this course's container is little-endian — exactly why the "
                  "shipped memcpy version writes the bytes backwards");

    std::uint8_t wire[6] = {};
    serialize({0x12345678u, 0xABCDu}, wire);

    // The wire's datasheet: most significant byte first.
    assert(wire[0] == 0x12 && wire[1] == 0x34 &&
           wire[2] == 0x56 && wire[3] == 0x78);
    assert(wire[4] == 0xAB && wire[5] == 0xCD);

    // Round trip: whatever we serialize, we can read back.
    SensorPacket back = deserialize(wire);
    assert(back.device_id == 0x12345678u && back.reading == 0xABCD);

    // A packet captured off the wire, sent by some other machine:
    const std::uint8_t captured[6] = {0x00, 0x00, 0xBE, 0xEF, 0x01, 0x02};
    SensorPacket cap = deserialize(captured);
    assert(cap.device_id == 0x0000BEEFu);
    assert(cap.reading == 0x0102);
    return 0;
}

// I AM NOT DONE
