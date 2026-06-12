// ranges2.cpp — solution

#include <cassert>
#include <ranges>
#include <vector>

struct Order {
    int cents;
    bool paid;
};

int transform_calls = 0;

int total_cents(const Order& o) {
    ++transform_calls;
    return o.cents;
}

bool is_paid(const Order& o) { return o.paid; }

int main() {
    std::vector<Order> orders = {
        {100, false}, {250, true}, {75, false}, {300, true}, {50, true},
    };

    auto paid_totals = orders | std::views::filter(is_paid)
                              | std::views::transform(total_cents);

    // 1. Building the pipeline computes NOTHING:
    assert(transform_calls == 0);

    // 2. One element consumed → exactly one transform call:
    int first = *paid_totals.begin();
    assert(first == 250);
    assert(transform_calls == 1);

    // 3. A full pass transforms the 3 paid orders; with the 1 from
    //    step 2 (views recompute — they store nothing): 4 total.
    int sum = 0;
    for (int cents : paid_totals) sum += cents;
    assert(sum == 600);
    assert(transform_calls == 4);
    return 0;
}
