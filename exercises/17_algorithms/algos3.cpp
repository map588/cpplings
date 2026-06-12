// algos3.cpp
//
// The sorted-range algorithms — binary_search, lower_bound, upper_bound,
// equal_range — buy O(log n) by ASSUMING sorted input. They do not
// check. On unsorted data they cheerfully bisect garbage and return a
// confident wrong answer; no crash, no sanitizer, nothing. The
// precondition is entirely your problem — that's the deal.
//
// The family, on a sorted range:
//   binary_search(f, l, x)   → bool: is x there?
//   lower_bound(f, l, x)     → iterator to first element >= x — which
//                              is exactly "where x belongs": the
//                              insert-in-sorted-order position
//   upper_bound(f, l, x)     → first element > x
//
// Task: the leaderboard code forgot one line. Find the function that
// has the unmet precondition and satisfy it. Then implement
// rank_position with lower_bound.

// I AM NOT DONE

#include <algorithm>
#include <cassert>
#include <vector>

// Index where `score` would slot into the (ascending) sorted scores:
std::size_t rank_position(const std::vector<int>& sorted_scores, int score) {
    return 0;   // TODO: lower_bound, then iterator arithmetic
}

int main() {
    std::vector<int> scores = {880, 120, 450, 990, 300};

    // O(log n) membership tests... on what, exactly?
    assert(std::binary_search(scores.begin(), scores.end(), 450));
    assert(!std::binary_search(scores.begin(), scores.end(), 451));

    assert(rank_position(scores, 100) == 0);    // before everything
    assert(rank_position(scores, 500) == 3);    // after 120, 300, 450
    assert(rank_position(scores, 2000) == 5);   // after everything
    return 0;
}
