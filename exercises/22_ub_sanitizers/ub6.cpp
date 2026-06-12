// ub6.cpp  — module capstone: shifts, and reading UBSan like a pro
//
// Bit shifts have a tight contract:
//
//   x << n   requires  0 <= n < width(x)     — shifting an int by 32
//                                              (or by -1) is UB
//   1 << 31  overflows: the result 2^31 doesn't fit in int. Remember
//            integral promotion (module 01): your uint8_t math happens
//            in INT, so "small type" shifts hit int's limits!
//
//            The fix is to shift an UNSIGNED value of sufficient width:
//            1u << 31 is fine; std::uint64_t{1} << n for wide masks.
//
// flag_mask() below commits both sins for the right inputs. UBSan
// reports each as it happens — fix them and re-run until silence:
//
//   "left shift of 1 by 31 places cannot be represented in type 'int'"
//   "shift exponent 32 is too large for 32-bit type 'int'"
//
// Task: make flag_mask correct for bit indices 0..63 by building the
// mask in std::uint64_t. The asserts pin down the edge cases that
// matter: bit 31 (the int trap) and bit 63 (the top).

// I AM NOT DONE

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
