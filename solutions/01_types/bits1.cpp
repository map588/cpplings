// bits1.cpp — solution

#include <cassert>
#include <cstdint>
#include <type_traits>

constexpr std::uint32_t ENABLE     = 1u << 0;               // bit 0: peripheral on/off
constexpr std::uint32_t LED        = 1u << 2;               // bit 2: status LED
constexpr std::uint32_t READY      = 1u << 3;               // bit 3: device ready
constexpr std::uint32_t MODE_SHIFT = 4;
constexpr std::uint32_t MODE_MASK  = 0b111u << MODE_SHIFT;  // bits 4-6: mode, 0..7
constexpr std::uint32_t IRQ        = 1u << 7;               // bit 7: interrupt pending

int main() {
    std::uint32_t reg = 0;  // power-on state: all bits zero

    // SET: OR the mask in.
    reg = reg | ENABLE;  // or: reg |= ENABLE;
    assert(reg == 0x01u);

    // (hardware simulation: the device comes up ready, and an interrupt fires)
    reg = reg | READY | IRQ;
    assert(reg == 0x89u);

    // TEST: AND isolates the bit; compare against zero.
    bool is_ready = (reg & READY) != 0;
    assert(is_ready);

    // CLEAR: AND with the inverted mask.
    reg = reg & ~IRQ;  // or: reg &= ~IRQ;
    assert(reg == 0x09u);

    // TOGGLE: XOR flips exactly the masked bits — both directions.
    reg = reg ^ LED;
    assert(reg == 0x0Du);
    reg = reg ^ LED;
    assert(reg == 0x09u);

    // FIELD WRITE: clear the old field bits FIRST, then OR in the new value.
    reg = (reg & ~MODE_MASK) | (5u << MODE_SHIFT);
    assert(reg == 0x59u);

    // FIELD READ: isolate, then shift down to bit 0.
    std::uint32_t mode = (reg & MODE_MASK) >> MODE_SHIFT;
    assert(mode == 5u);

    // FIELD REWRITE: the clear step is what keeps 0b101 from merging into 0b010.
    reg = (reg & ~MODE_MASK) | (2u << MODE_SHIFT);
    assert(reg == 0x29u);
    assert(((reg & MODE_MASK) >> MODE_SHIFT) == 2u);

    // The 8-bit wrinkle: ~ctrl is int math (promotion), so truncate back
    // to 8 bits explicitly.
    std::uint8_t ctrl = 0xA5;
    static_assert(std::is_same_v<decltype(~ctrl), int>);
    std::uint8_t inverted = static_cast<std::uint8_t>(~ctrl);
    assert(inverted == 0x5A);

    return 0;
}
