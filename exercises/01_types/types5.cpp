// types5.cpp
//
// Plain `int` guarantees only "at least 16 bits". `long` is 32 bits on
// 64-bit Windows and 64 bits on 64-bit Linux/macOS. When width matters —
// file formats, protocols, hardware registers — say the width out loud.
//
// <cstdint> (C++11) gives you:
//   int8_t / uint8_t ... int64_t / uint64_t   — exactly N bits
//   int_least8_t ...                          — smallest type with ≥ N bits
//   int_fast8_t ...                           — fastest type with ≥ N bits
//   intptr_t / uintptr_t                      — wide enough to hold a pointer
//
// And <cstddef> gives std::size_t: unsigned, wide enough for any object
// size — it's what sizeof and .size() return.
//
// Task: replace each alias's type with the SMALLEST exact-width type that
// satisfies the stated requirement. The static_asserts encode the
// requirements.

// I AM NOT DONE

#include <cstdint>
#include <type_traits>

// Holds temperatures from -200 to +200 (whole degrees).
using temperature_t = std::int64_t;

// Holds one 8-bit pixel channel: 0 to 255.
using pixel_t = std::int64_t;

// Holds a count of microseconds up to ~584,000 years (needs 64 bits, signed).
using usec_t = std::int8_t;

static_assert(sizeof(temperature_t) == 2 && std::is_signed_v<temperature_t>,
              "temperature: smallest signed type that fits -200..200");
static_assert(sizeof(pixel_t) == 1 && std::is_unsigned_v<pixel_t>,
              "pixel: smallest unsigned type that fits 0..255");
static_assert(sizeof(usec_t) == 8 && std::is_signed_v<usec_t>,
              "usec: 64-bit signed");

int main() { return 0; }
