// errors4.cpp
//
// The exception-safety ladder — what your callers may assume when you
// throw:
//
//   NOTHROW   can't fail (noexcept)
//   STRONG    failed → NOTHING changed. Commit-or-rollback.
//   BASIC     failed → object still valid, nothing leaked... but in an
//             unspecified state. (RAII members give you this for free.)
//   none      failed → invariants broken. A bug wearing a try-block.
//
// Inventory::load_crate below offers only BASIC: it mutates as it goes,
// and when the weight check throws mid-crate, half the items are
// already in. Valid object, garbage contents — the caller can't retry,
// can't trust totals, can only clear and start over.
//
// THE STRONG-GUARANTEE RECIPE: do everything that can throw OFF TO THE
// SIDE, then commit with operations that cannot throw:
//
//     1. build the new state in temporaries     (may throw — who cares,
//                                                *this is untouched)
//     2. validate                                (may throw — same)
//     3. commit via swap / move-assign           (noexcept — point of
//                                                no return, but no
//                                                failures past it)
//
// Task: give load_crate the strong guarantee. The post-throw asserts
// define "nothing changed".

// I AM NOT DONE

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

class Inventory {
public:
    void load_crate(const std::vector<std::string>& names,
                    const std::vector<int>& weights) {
        for (std::size_t i = 0; i < names.size(); ++i) {
            items_.push_back(names[i]);          // mutating before...
            total_weight_ += weights[i];
            if (total_weight_ > kCapacity) {     // ...we know it fits
                throw std::overflow_error("crate exceeds capacity");
            }
        }
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
        inv.load_crate({"feather", "piano"}, {1, 90});   // 65+1+90 > 100
    } catch (const std::overflow_error&) {
        threw = true;
    }
    assert(threw);

    // STRONG guarantee: the failed crate left no trace —
    assert(inv.item_count() == 2);        // no half-loaded "feather"
    assert(inv.total_weight() == 65);     // weight rolled back too
    return 0;
}
