// algos3.cpp — solution

#include <algorithm>
#include <cassert>
#include <vector>

std::size_t rank_position(const std::vector<int>& sorted_scores, int score) {
    auto it = std::lower_bound(sorted_scores.begin(), sorted_scores.end(),
                               score);
    return static_cast<std::size_t>(it - sorted_scores.begin());
}

int main() {
    std::vector<int> scores = {880, 120, 450, 990, 300};

    std::sort(scores.begin(), scores.end());    // THE missing line

    assert(std::binary_search(scores.begin(), scores.end(), 450));
    assert(!std::binary_search(scores.begin(), scores.end(), 451));

    assert(rank_position(scores, 100) == 0);
    assert(rank_position(scores, 500) == 3);
    assert(rank_position(scores, 2000) == 5);
    return 0;
}
