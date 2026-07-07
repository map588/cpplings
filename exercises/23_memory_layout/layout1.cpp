// layout1.cpp
//
// Two rules generate every struct layout:
//
//   1. each member sits at an offset divisible by its alignof
//   2. the struct's size rounds up to a multiple of its strictest
//      member alignment (so arrays of it stay aligned)
//
// The compiler may NOT reorder members for you (declaration order is
// layout order — C compatibility). Wherever the next member's
// alignment doesn't divide the current offset, the compiler inserts
// PADDING — invisible bytes you pay for in every element of every
// array. A million-element vector of a padding-heavy struct: megabytes
// of air and extra cache misses, all avoidable by declaring the same
// members in a smarter order.
//
// Apply the two rules to Wasteful below, member by member, offset by
// offset (typical 64-bit platform — this course's: alignof(int) == 4,
// alignof(double) == 8). Then find the ordering principle that makes
// padding evaporate — it's mechanical once you see it.
//
// Task: replace each TODO with the number the two rules predict, then
// reorder Packed's members so its static_assert holds.
//   - the file compiles: the static_asserts are the checker
// Constraints:
//   - Wasteful's member order stays exactly as declared
//   - Packed keeps the same four members (names and types) — only
//     their order is yours
//   - no pragmas or attributes — declaration order is the only tool
//   - Packed's static_assert keeps its 16; on Wasteful, change only
//     the TODOs

#include <cstddef>

constexpr std::size_t TODO = 9999;

struct Wasteful {
    char tag;
    double value;
    char flag;
    int count;
};

static_assert(alignof(double) == TODO);
static_assert(offsetof(Wasteful, value) == TODO);   // pushed where by tag?
static_assert(sizeof(Wasteful) == TODO);

// Same four members — order them large-to-small:
struct Packed {
    char tag;
    double value;
    char flag;
    int count;
};

static_assert(sizeof(Packed) == 16, "8 + 4 + 1 + 1 + 2 tail padding");

int main() { return 0; }

// I AM NOT DONE
