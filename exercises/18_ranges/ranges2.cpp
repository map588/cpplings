// ranges2.cpp
//
// Views are LAZY. This line does no filtering and no transforming:
//
//     auto pipeline = orders | std::views::filter(is_paid)
//                            | std::views::transform(total_cents);
//
// It builds a recipe. Work happens during ITERATION, element by
// element, pull-driven: each `++it` advances through filter (skipping
// rejects) and applies transform to the one element you're looking at.
// Nothing is stored; iterate twice, compute twice.
//
// This file PROVES the laziness with a call counter inside the
// transform function. Predict the counter at each checkpoint —
// internalizing this execution model is the whole exercise.
//
// (Note: consuming just the FIRST element still forces the filter to
// CHURN through rejects until it reaches a paid order — laziness saves
// transform calls, not the filter's scanning.)
//
// Task: replace each TODO with the number the counter really holds at
// that checkpoint.
//   - the program compiles and every assert passes
//   - derive each value from the execution model above BEFORE running —
//     the compiler confirming your prediction is the exercise
// Constraints:
//   - do not touch the pipeline, the counter, or total_cents
//   - do not delete or reorder the asserts

#include <cassert>
#include <ranges>
#include <vector>

constexpr int TODO = -1;

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

    // 1. The pipeline exists. How much work has happened?
    assert(transform_calls == TODO);

    // 2. Consume ONE element:
    int first = *paid_totals.begin();
    assert(first == 250);
    assert(transform_calls == TODO);

    // 3. Now consume the whole pipeline:
    int sum = 0;
    for (int cents : paid_totals) sum += cents;
    assert(sum == 600);
    assert(transform_calls == TODO);   // total after 1 + a full pass
    return 0;
}

// I AM NOT DONE
