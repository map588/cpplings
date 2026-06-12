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
// Task: reimplement all three function bodies with the algorithms. No
// `for`, no `while`.

// I AM NOT DONE

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

double total_weight(const std::vector<double>& weights) {
    int sum = 0;                             // spot the bug — and notice
    for (double w : weights) sum += w;       // accumulate(..., 0) would
    return sum;                              // keep it! Use 0.0.
}

double heaviest(const std::vector<double>& weights) {
    double best = 0;
    for (double w : weights) {               // TODO: max_element
        if (w > best) best = w;
    }
    return best;
}

bool carries(const std::vector<double>& weights, double exact) {
    for (double w : weights) {               // TODO: find
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
