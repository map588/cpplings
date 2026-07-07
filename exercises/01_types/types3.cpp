// types3.cpp
//
// Integral promotion: before ANY arithmetic happens, operands smaller than
// `int` (char, short, uint8_t, ...) are promoted to `int`. There is no such
// thing as uint8_t + uint8_t arithmetic in C++ — the machine may do it, the
// language does not.
//
// Consequences:
//   - adding two uint8_t values is `int` math — the sum does NOT wrap at
//     256. Wrapping happens only if you stuff the result back into a
//     uint8_t yourself (and then it's the value modulo 256).
//   - unsigned arithmetic WRAPS, and that's well-defined behavior.
//   - signed overflow is UNDEFINED behavior. (INT_MAX + 1 is not "a negative
//     number" — it's a license for the optimizer to assume it never happens.)
//
// Task: replace each use of TODO with the value the program actually
// computes. Predict before you run!
//   - compiles; every assert passes; exits 0
// Constraints:
//   - change only the TODO occurrences — not the declarations, the cast,
//     or any assert's left-hand side


#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

constexpr int TODO = -1;  // replace each use of TODO with the real value

int main() {
    std::uint8_t a = 200;
    std::uint8_t b = 100;

    auto sum = a + b;  // promoted: this is int arithmetic
    static_assert(std::is_same_v<decltype(sum), int>);
    assert(sum == TODO);

    // Truncation only happens when YOU put it back in 8 bits (mod 256):
    auto wrapped = static_cast<std::uint8_t>(a + b);
    assert(wrapped == TODO);

    // Unsigned wraparound is well-defined: 0 - 1 wraps to the maximum.
    unsigned int zero = 0;
    assert(zero - 1 == std::numeric_limits<unsigned int>::max());  // true as-is!

    // But the same idea with signed ints would be UB, not a wrap:
    //   int i = std::numeric_limits<int>::max();
    //   ++i;   // undefined behavior — try it in the UB module, with UBSan on
    return 0;
}
// I AM NOT DONE
