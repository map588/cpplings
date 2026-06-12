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
// The LEGAL transmuters, in order of preference:
//
//     std::bit_cast<To>(from)        // C++20: constexpr-friendly,
//                                    // sizes must match, no UB
//     std::memcpy(&to, &from, n)     // the classic; compilers compile
//                                    // it to a plain register move
//
// Task: parse_u32 reads a u32 from byte OFFSET 1 of a packet through a
// casted pointer. Fix it with memcpy (or bit_cast from a copied array).

// I AM NOT DONE

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
