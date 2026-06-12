// layout4.cpp — solution

#include <cstddef>

struct ByStart {
    bool operator()(int a, int b) const { return a < b; }
};

struct Timeline {
    [[no_unique_address]] ByStart cmp;   // zero bytes: it has no state
    double* events;
    std::size_t count;
};

static_assert(sizeof(Timeline) == 16,
              "the empty comparator should cost nothing");

struct Empty {};
struct Holder { Empty e; char c; };

static_assert(sizeof(Empty) == 1);      // unique-address rule
static_assert(sizeof(Holder) == 2);     // e at 0, c at 1 — both need a byte

int main() { return 0; }
