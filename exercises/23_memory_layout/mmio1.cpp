// mmio1.cpp
//
// Memory-mapped I/O: a peripheral's registers ARE memory. The datasheet
// hands you a table of byte offsets from a base address, and firmware
// overlays a struct on it:
//
//     auto* pulse = reinterpret_cast<volatile PulseRegs*>(0x4002'0800);
//     pulse->PRESCALE = 8;             // a bus write to base + 0x04
//
// (volatile so every access really happens. We practice the layout
// contract on a plain object — it's the same contract either way.)
//
// The overlay is only correct if every member lands EXACTLY on its
// datasheet offset. Layout is deterministic — declaration order is
// layout order, and the compiler inserts only the MINIMUM padding that
// alignment demands (layout1's rules). It will never invent the gaps a
// datasheet leaves reserved: between two u8 registers alignment demands
// nothing, so if the map says there's a hole, YOU declare it, as a
// member. (The other tool people reach for, #pragma pack, is the wrong
// one here: it deletes gaps rather than adding them, and packed members
// count as potentially misaligned — on strict-alignment ARM cores the
// compiler then emits byte-by-byte loads. Explicit reserved members
// cost nothing.)
//
// The contract is checkable at compile time: offsetof (from C, in
// <cstddef>) yields a member's byte offset — guaranteed only for
// STANDARD-LAYOUT types (no virtual functions, no mixing access
// specifiers across data members; std::is_standard_layout (C++11)
// verifies). The datasheet:
//
//     PULSE-8 pulse counter — register block
//       0x00  u32  CTRL       enable / mode bits
//       0x04  u8   PRESCALE   clock divider
//       0x05  u8   (reserved)
//       0x06  u8   IRQ_MASK   interrupt enables
//       0x07  u8   (reserved)
//       0x08  u32  COUNT      pulse count (read-only)
//       0x0C  u32  (reserved — future use)
//     block size 0x10; four PULSE-8 blocks tile back-to-back.
//
// Task: make PulseRegs honor the datasheet.
//   - every offsetof/sizeof static_assert holds (they encode the table)
//   - the runtime byte-inspection asserts pass; ASan/UBSan stay green
// Constraints:
//   - don't change any static_assert or assert
//   - keep registers in datasheet order; add only what the table shows
//   - no #pragma pack, no alignas — plain member declarations only

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

struct PulseRegs {
    std::uint32_t CTRL;       // datasheet: 0x00
    std::uint8_t  PRESCALE;   // datasheet: 0x04
    std::uint8_t  IRQ_MASK;   // datasheet: 0x06 — is that where this is?
    std::uint32_t COUNT;      // datasheet: 0x08 — padding got lucky here
};

// The datasheet, spelled as compile-time checks — one per register:
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

    // Ground truth for the overlay: view the object as raw bytes
    // (the unsigned char* view of an object is always legal — no UB).
    const auto* bytes = reinterpret_cast<const unsigned char*>(&r);
    assert(bytes[0x04] == 8    && "PRESCALE must live at byte 0x04");
    assert(bytes[0x06] == 0x01 && "IRQ_MASK must live at byte 0x06");
    return 0;
}

// I AM NOT DONE
