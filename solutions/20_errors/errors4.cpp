// errors4.cpp — solution

#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class Inventory {
public:
    void load_crate(const std::vector<std::string>& names,
                    const std::vector<int>& weights) {
        // 1. Build the candidate state on the side (may throw: fine).
        std::vector<std::string> new_items = items_;
        int new_weight = total_weight_;
        for (std::size_t i = 0; i < names.size(); ++i) {
            new_items.push_back(names[i]);
            new_weight += weights[i];
        }
        // 2. Validate (may throw: *this still untouched).
        if (new_weight > kCapacity) {
            throw std::overflow_error("crate exceeds capacity");
        }
        // 3. Commit with nothrow operations.
        items_ = std::move(new_items);
        total_weight_ = new_weight;
    }

    std::size_t item_count() const { return items_.size(); }
    int total_weight() const { return total_weight_; }

private:
    static constexpr int kCapacity = 100;
    std::vector<std::string> items_;
    int total_weight_ = 0;
};

int main() {
    Inventory inv;
    inv.load_crate({"anvil", "rope"}, {60, 5});
    assert(inv.item_count() == 2 && inv.total_weight() == 65);

    bool threw = false;
    try {
        inv.load_crate({"feather", "piano"}, {1, 90});
    } catch (const std::overflow_error&) {
        threw = true;
    }
    assert(threw);

    assert(inv.item_count() == 2);
    assert(inv.total_weight() == 65);
    return 0;
}
