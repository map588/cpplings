// layout1.cpp
//
// Two rules generate every struct layout:
//
//   1. each member sits at an offset divisible by its alignof
//   2. the struct's size rounds up to a multiple of its strictest
//      member alignment (so arrays of it stay aligned)
//
// The compiler may NOT reorder members for you (declaration order is
// layout order — C compatibility). So Wasteful below goes:
//
//     char tag;        offset 0, then 7 bytes PADDING (double wants 8)
//     double value;    offset 8
//     char flag;       offset 16, then 3 padding (int wants 4)
//     int count;       offset 20 → size 24
//
// 10 of 24 bytes are air. The cure is mechanical: sort members
// LARGE-TO-SMALL and the padding evaporates. A million-element vector
// of these: 8 MB and a third of the cache misses, for a cut-and-paste.
//
// Task: fill the TODOs for Wasteful (the numbers assume a typical
// 64-bit platform — this course's). Then order Packed's members so the
// final static_assert holds.

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
