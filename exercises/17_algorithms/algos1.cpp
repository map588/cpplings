// algos1.cpp
//
// Three hand-rolled loops, three standard algorithms waiting to replace
// them:
//
//   std::accumulate(first, last, init)        // <numeric>!
//       Caution: the INIT ARGUMENT'S TYPE is the accumulator's type.
//       accumulate(..., 0) on doubles truncates every step — pass 0.0.
//
//   std::max_element(first, last)
//       Returns an ITERATOR to the max (end() if the range is empty —
//       check before dereferencing). *it for the value.
//
//   std::find(first, last, value)
//       Iterator to the first match, or end(). The found-ness test IS
//       the end() comparison.
//
// Why bother? The names state intent at a glance, the implementations
// have survived thirty years of code review, and the iterator interface
// means they work on any container — list, deque, array — unchanged.
//
// Task: reimplement all three function bodies with standard algorithms.
//   - every assert passes — note total_weight() must return 7.5, not 6:
//     the hand-rolled loop's accumulator bug is exactly the trap the
//     algorithm version can inherit if you let it
//   - heaviest() must stay safe for an EMPTY vector (return 0.0): an
//     algorithm that answers with an iterator makes you decide what
//     end() means before dereferencing
// Constraints:
//   - no `for`, no `while`, no manual loops of any kind
//   - one algorithm call per function
//   - don't change the signatures or the asserts

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

double total_weight(const std::vector<double>& weights) {
    int sum = 0;                             // spot the accumulator bug —
    for (double w : weights) sum += w;       // don't carry it across
    return sum;
}

double heaviest(const std::vector<double>& weights) {
    double best = 0;
    for (double w : weights) {               // TODO
        if (w > best) best = w;
    }
    return best;
}

bool carries(const std::vector<double>& weights, double exact) {
    for (double w : weights) {               // TODO
        if (w == exact) return true;
    }
    return false;
}

int main() {
    std::vector<double> cargo = {0.5, 2.25, 1.0, 3.75};

    assert(total_weight(cargo) == 7.5);      // 0 init would say 6!
    assert(heaviest(cargo) == 3.75);
    assert(carries(cargo, 2.25));
    assert(!carries(cargo, 9.99));
    return 0;
}

// I AM NOT DONE
