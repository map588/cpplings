// bits1.cpp
//
// Memory-mapped I/O: on a microcontroller, a peripheral (UART, timer, GPIO
// port...) is configured through a STATUS/CONTROL REGISTER — a fixed-width
// integer at a fixed address where every bit, or small group of bits, means
// something ("bit 3: device ready", "bits 4-6: operating mode"). There is
// no struct and no setter: you edit individual bits in place with the
// bitwise operators C++ inherited from C:
//
//   reg | mask    bits that are 1 in mask come out 1   (set)
//   reg & mask    bits that are 0 in mask come out 0   (keep-only / test)
//   reg ^ mask    bits that are 1 in mask flip         (toggle)
//   ~mask         every bit inverted                   (turn a set-mask
//                                                       into a clear-mask)
//   1u << n       a mask with only bit n set
//
// A MULTI-BIT FIELD (like a 3-bit mode) works with mask + shift: to read,
// isolate the field's bits and shift them down to bit 0; to write, clear
// the field's OLD bits first, then OR in the new value shifted up. Skipping
// the clear is the classic firmware bug: old and new bits merge.
//
// One wrinkle for 8-bit registers: the bitwise operators compute in int.
// Operands smaller than int are promoted first (types3 has the full story),
// so ~ on a uint8_t yields a 32-bit int with 24 extra 1-bits on top — you
// must put the result back into 8 bits yourself, and deliberately.
//
// The constants below use fixed-width types from <cstdint> (C++11) and
// constexpr (C++11) — exactly how real device headers are written.
//
// Task: replace every TODO expression so the program compiles and passes.
//   - compiles; every assert and static_assert passes; exits 0
// Constraints:
//   - build each TODO from `reg`/`ctrl` and the named constants; the only
//     numeric literals you may introduce are the mode numbers 5 and 2 —
//     never a literal that just equals an assert's expected value
//   - don't change the constants, the "hardware simulation" lines, or any
//     assert


#include <cassert>
#include <cstdint>
#include <type_traits>

// The register layout, as the vendor's header would define it:
constexpr std::uint32_t ENABLE     = 1u << 0;               // bit 0: peripheral on/off
constexpr std::uint32_t LED        = 1u << 2;               // bit 2: status LED
constexpr std::uint32_t READY      = 1u << 3;               // bit 3: device ready
constexpr std::uint32_t MODE_SHIFT = 4;
constexpr std::uint32_t MODE_MASK  = 0b111u << MODE_SHIFT;  // bits 4-6: mode, 0..7
constexpr std::uint32_t IRQ        = 1u << 7;               // bit 7: interrupt pending

int main() {
    std::uint32_t reg = 0;  // power-on state: all bits zero

    // --- SET: switch the peripheral on. Only ENABLE may change.
    reg = 0;  // TODO
    assert(reg == 0x01u);

    // (hardware simulation: the device comes up ready, and an interrupt fires)
    reg = reg | READY | IRQ;
    assert(reg == 0x89u);

    // --- TEST: proceed only if the device reports ready.
    bool is_ready = false;  // TODO: true exactly when READY is set in reg
    assert(is_ready);

    // --- CLEAR: acknowledge the interrupt — IRQ off, every other bit kept.
    reg = 0;  // TODO
    assert(reg == 0x09u);

    // --- TOGGLE: blink the status LED — flip it on...
    reg = 0;  // TODO
    assert(reg == 0x0Du);
    // ...and the exact same expression flips it back off:
    reg = 0;  // TODO
    assert(reg == 0x09u);

    // --- FIELD WRITE: select mode 5 (0b101) in bits 4-6, touching nothing else.
    reg = 0;  // TODO
    assert(reg == 0x59u);

    // --- FIELD READ: which mode is the device in? (a number 0..7)
    std::uint32_t mode = 0;  // TODO: extract the field, shifted down to bit 0
    assert(mode == 5u);

    // --- FIELD REWRITE: switch to mode 2 (0b010). If your write doesn't
    //     clear the old bits first, 0b101 | 0b010 = 0b111 — mode 7. Oops.
    reg = 0;  // TODO
    assert(reg == 0x29u);
    assert(((reg & MODE_MASK) >> MODE_SHIFT) == 2u);

    // --- the 8-bit wrinkle: ~ happens in int (promotion — see types3).
    std::uint8_t ctrl = 0xA5;
    static_assert(std::is_same_v<decltype(~ctrl), int>);  // true as-is!
    std::uint8_t inverted = 0;  // TODO: every bit of ctrl flipped, back in 8 bits
    assert(inverted == 0x5A);

    return 0;
}
// I AM NOT DONE
