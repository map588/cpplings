// layout1.cpp — solution

#include <cstddef>

struct Wasteful {
    char tag;
    double value;
    char flag;
    int count;
};

static_assert(alignof(double) == 8);
static_assert(offsetof(Wasteful, value) == 8);   // 1 byte tag + 7 padding
static_assert(sizeof(Wasteful) == 24);

// Large-to-small: alignments nest, padding vanishes.
struct Packed {
    double value;
    int count;
    char tag;
    char flag;
};

static_assert(sizeof(Packed) == 16, "8 + 4 + 1 + 1 + 2 tail padding");

int main() { return 0; }
