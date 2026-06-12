// ranges4.cpp — solution

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

std::vector<int> load_scores() {
    return {88, 42, 95, 61};
}

int main() {
    // Name the owner; iterators into it are now safe to hold.
    std::vector<int> scores = load_scores();
    auto best = std::ranges::max_element(scores);
    assert(*best == 95);
    return 0;
}
