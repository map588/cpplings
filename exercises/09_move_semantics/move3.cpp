// move3.cpp
//
// Why all the nagging about noexcept on move members? Because
// std::vector is listening.
//
// When a vector grows, it must transfer existing elements to the new,
// bigger allocation. Moving them is fast — but if a move constructor
// CAN throw, a throw halfway through leaves half the elements moved out
// of the old buffer: the strong exception guarantee ("growth either
// succeeds or changes nothing") would be broken, and vector promises it.
//
// So vector transfers with std::move_if_noexcept:
//
//   move ctor noexcept  →  elements are MOVED
//   move ctor may throw →  elements are COPIED. Every element. Every
//                          reallocation. Forever. Silently.
//
// One missing keyword turns O(n) pointer swaps into O(n) deep copies —
// this single rule is most of the practical reason "moves must be
// noexcept" is drilled into C++ codebases.
//
// Task: Item's move constructor is correct but unmarked. Watch the
// counters, then add the keyword.

// I AM NOT DONE

#include <cassert>
#include <string>
#include <vector>

struct Item {
    inline static int copies = 0;
    inline static int moves = 0;

    std::string payload;

    explicit Item(const char* p) : payload(p) {}
    Item(const Item& o) : payload(o.payload) { ++copies; }
    Item(Item&& o) : payload(std::move(o.payload)) { ++moves; }
    Item& operator=(const Item&) = default;
    Item& operator=(Item&&) = default;
};

int main() {
    std::vector<Item> v;
    v.reserve(2);
    v.emplace_back("first");       // constructed in place: no copy, no move
    v.emplace_back("second");      // capacity now full

    Item::copies = 0;
    Item::moves = 0;

    v.emplace_back("third");       // reallocation! the 2 old elements
                                   // must transfer to the new buffer

    assert(Item::moves == 2);      // moved... if vector trusts the move
    assert(Item::copies == 0);
    return 0;
}
