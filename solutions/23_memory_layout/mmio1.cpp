// mmio1.cpp — solution
// The compiler only pads where alignment demands; datasheet holes must
// be explicit reserved members. No #pragma pack needed — or wanted.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

struct PulseRegs {
    std::uint32_t CTRL;       // 0x00
    std::uint8_t  PRESCALE;   // 0x04
    std::uint8_t  reserved0;  // 0x05 — the datasheet's hole, spelled out
    std::uint8_t  IRQ_MASK;   // 0x06
    std::uint8_t  reserved1;  // 0x07
    std::uint32_t COUNT;      // 0x08
    std::uint32_t reserved2;  // 0x0C — future use, still part of the tile
};

static_assert(std::is_standard_layout_v<PulseRegs>,
              "offsetof is only guaranteed for standard-layout types");
static_assert(offsetof(PulseRegs, CTRL)     == 0x00);
static_assert(offsetof(PulseRegs, PRESCALE) == 0x04);
static_assert(offsetof(PulseRegs, IRQ_MASK) == 0x06,
              "the hole at 0x05 is yours to declare");
static_assert(offsetof(PulseRegs, COUNT)    == 0x08);
static_assert(sizeof(PulseRegs) == 0x10,
              "blocks tile back-to-back — the trailing reserved word counts");

int main() {
    PulseRegs r{};
    r.CTRL     = 1;
    r.PRESCALE = 8;
    r.IRQ_MASK = 0x01;

    const auto* bytes = reinterpret_cast<const unsigned char*>(&r);
    assert(bytes[0x04] == 8    && "PRESCALE must live at byte 0x04");
    assert(bytes[0x06] == 0x01 && "IRQ_MASK must live at byte 0x06");
    return 0;
}
