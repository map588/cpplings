// types5.cpp — solution

#include <cstdint>
#include <type_traits>

// -200..200 needs more than 8 bits (int8_t tops out at 127), so 16:
using temperature_t = std::int16_t;

// 0..255 is exactly what uint8_t holds:
using pixel_t = std::uint8_t;

// 64-bit signed:
using usec_t = std::int64_t;

static_assert(sizeof(temperature_t) == 2 && std::is_signed_v<temperature_t>,
              "temperature: smallest signed type that fits -200..200");
static_assert(sizeof(pixel_t) == 1 && std::is_unsigned_v<pixel_t>,
              "pixel: smallest unsigned type that fits 0..255");
static_assert(sizeof(usec_t) == 8 && std::is_signed_v<usec_t>,
              "usec: 64-bit signed");

int main() { return 0; }
