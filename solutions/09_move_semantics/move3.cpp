// move3.cpp — solution

#include <cassert>
#include <string>
#include <vector>

struct Item {
    inline static int copies = 0;
    inline static int moves = 0;

    std::string payload;

    explicit Item(const char* p) : payload(p) {}
    Item(const Item& o) : payload(o.payload) { ++copies; }
    Item(Item&& o) noexcept : payload(std::move(o.payload)) { ++moves; }
    Item& operator=(const Item&) = default;
    Item& operator=(Item&&) = default;
};

int main() {
    std::vector<Item> v;
    v.reserve(2);
    v.emplace_back("first");
    v.emplace_back("second");

    Item::copies = 0;
    Item::moves = 0;

    v.emplace_back("third");       // move_if_noexcept now picks the moves

    assert(Item::moves == 2);
    assert(Item::copies == 0);
    return 0;
}
