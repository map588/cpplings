// ranges3.cpp — solution

#include <cassert>
#include <ranges>
#include <vector>

std::vector<int> perfect_squares_ending_in_6(int k) {
    auto pipeline = std::views::iota(1)
                  | std::views::transform([](int n) { return n * n; })
                  | std::views::filter([](int sq) { return sq % 10 == 6; })
                  | std::views::take(k);

    std::vector<int> out;
    for (int x : pipeline) out.push_back(x);
    return out;
    // C++23: return pipeline | std::ranges::to<std::vector>();
}

int main() {
    assert(perfect_squares_ending_in_6(1) == (std::vector<int>{16}));
    assert(perfect_squares_ending_in_6(4)
           == (std::vector<int>{16, 36, 196, 256}));
    return 0;
}
