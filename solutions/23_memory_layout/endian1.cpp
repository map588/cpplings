// endian1.cpp — solution
// Shift-and-mask works on values, so it emits the same wire bytes on
// any host — no endianness branch, no byteswap.

#include <bit>
#include <cassert>
#include <cstdint>

struct SensorPacket {
    std::uint32_t device_id;
    std::uint16_t reading;
};

void serialize(const SensorPacket& p, std::uint8_t out[6]) {
    out[0] = static_cast<std::uint8_t>(p.device_id >> 24);
    out[1] = static_cast<std::uint8_t>(p.device_id >> 16);
    out[2] = static_cast<std::uint8_t>(p.device_id >> 8);
    out[3] = static_cast<std::uint8_t>(p.device_id);
    out[4] = static_cast<std::uint8_t>(p.reading >> 8);
    out[5] = static_cast<std::uint8_t>(p.reading);
}

SensorPacket deserialize(const std::uint8_t in[6]) {
    SensorPacket p{};
    p.device_id = (std::uint32_t{in[0]} << 24) |
                  (std::uint32_t{in[1]} << 16) |
                  (std::uint32_t{in[2]} << 8)  |
                   std::uint32_t{in[3]};
    // Shift in a wide unsigned type here too: bare in[4] promotes to
    // int, and on a 16-bit-int target 0xAB << 8 would overflow it (UB).
    p.reading   = static_cast<std::uint16_t>((std::uint32_t{in[4]} << 8) | in[5]);
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
