// ub4.cpp
//
// Reinterpreting bytes through a casted pointer breaks TWO promises at
// once:
//
//   ALIGNMENT — an int* must point at int-aligned memory (addresses
//   divisible by alignof(int)). `buffer + 1` isn't. Some CPUs fault on
//   misaligned loads; others silently tolerate them until a vectorized
//   path doesn't. UBSan checks every access — watch it name this line.
//
//   STRICT ALIASING — the optimizer assumes a `T*` and an unrelated
//   `U*` never alias, and reorders/caches accordingly. Reading a float
//   through an int* (the old "fast inverse sqrt" trick) is UB that
//   often survives -O0 and dies at -O2, in a different function,
//   years later. (char*/std::byte* are the blessed exception — they
//   may alias anything.)
//
// The lesson: you may not LIE about a pointer's type — but you may
// always COPY bytes into a real object of the type you want. The
// standard library has blessed transmuters for exactly this (one as
// old as C, one new in C++20); optimizers turn them into the single
// load you were trying to write anyway. This file already includes
// the header one of them lives in.
//
// Task: make parse_payload return the u32 stored at byte offset 1.
//   - the assert passes (0x12345678)
//   - runs clean under the sanitizers (no misaligned-load report)
// Constraints:
//   - keep the packet bytes and the assert
//   - no reinterpret_cast of the payload pointer to uint32_t* — the
//     cast IS the bug, not a thing to decorate

#include <cassert>
#include <cstdint>
#include <cstring>

// Packet layout: 1-byte tag, then a little-endian u32 payload.
std::uint32_t parse_payload(const unsigned char* packet) {
    const std::uint32_t* p =
        reinterpret_cast<const std::uint32_t*>(packet + 1);   // misaligned!
    return *p;
}

int main() {
    unsigned char packet[5] = {0x7F, 0x78, 0x56, 0x34, 0x12};

    assert(parse_payload(packet) == 0x12345678);
    return 0;
}

// I AM NOT DONE
