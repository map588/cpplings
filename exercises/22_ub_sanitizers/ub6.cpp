// ub6.cpp  — module capstone: shifts, and reading UBSan like a pro
//
// Bit shifts have a tight contract — and C++20 rewrote half of it:
//
//   x << n   requires  0 <= n < width(x)     — shifting an int by 32
//                                              (or by -1) is UB. Still.
//   1 << 31  the result 2^31 doesn't fit in int. In C, and in early
//            C++, that was UB too. C++20 DEFINED it: a signed left
//            shift now wraps, so 1 << 31 is INT_MIN — a negative int.
//            No UB, no sanitizer report... and no mercy: widen that
//            int to 64 bits and the sign comes along for the ride,
//            0xFFFFFFFF80000000. Defined behavior, wrong mask.
//            (Integral promotion, module 01, drags "small type"
//            shifts into int too — your uint8_t math hits int's limits.)
//
// flag_mask() below commits both sins for the right inputs, and they
// fail DIFFERENTLY — learn to tell the two apart:
//
//   bit 31   an assert fails, quietly. Nothing was undefined, so UBSan
//            has nothing to say; the value is simply wrong.
//   bit 32+  UBSan speaks up:
//            "shift exponent 63 is too large for 32-bit type 'int'"
//
// Both failures blame the same thing — not the shift count, the
// OPERAND. Ask what type the literal `1` has, and therefore what type
// the shift happens in. The fix is a one-liner.
//
// Task: make flag_mask correct for every bit index in [0, 64).
//   - all four asserts pass — bit 31 (the int trap) and bit 63 (the
//     top) are the edge cases that matter
//   - runs clean under the sanitizers (no shift-exponent report)
// Constraints:
//   - keep the signature and the asserts
//   - no branches, no tables — fix the type the shift happens in

#include <cassert>
#include <cstdint>

// A mask with bit `bit` set (bit ∈ [0, 64)).
std::uint64_t flag_mask(int bit) {
    return 1 << bit;
}

int main() {
    assert(flag_mask(0) == 1);
    assert(flag_mask(4) == 16);

    assert(flag_mask(31) == 0x80000000ull);            // the int trap
    assert(flag_mask(63) == 0x8000000000000000ull);    // the top bit
    return 0;
}

// I AM NOT DONE
