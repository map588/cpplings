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
// Task: make every assert pass — honestly.
//   - the two binary_search asserts must be TRUE ANSWERS about the
//     data: something in main() leaves a precondition unmet, and the
//     confident wrong answers start there. Find it, satisfy it
//   - implement rank_position(): the index where `score` would slot
//     into the ascending scores — in O(log n)
// Constraints:
//   - keep binary_search doing the membership tests; don't reorder or
//     change the asserts
//   - no loops or linear scans in rank_position()
//   - the missing-precondition fix is ONE line in main()

#include <algorithm>
#include <cassert>
#include <vector>

// Index where `score` would slot into the (ascending) sorted scores:
std::size_t rank_position(const std::vector<int>& sorted_scores, int score) {
    return 0;   // TODO
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

// I AM NOT DONE
