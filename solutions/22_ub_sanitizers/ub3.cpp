// ub3.cpp — solution

#include <cassert>
#include <vector>

int main() {
    std::vector<int> entries = {12, 47, 8, 31};

    // Track the value: it outlives every iteration.
    bool found = false;
    int best = 0;
    for (std::size_t i = 0; i < entries.size(); ++i) {
        int score = entries[i] * 2;
        if (!found || score > best) {
            best = score;
            found = true;
        }
    }

    assert(best == 94);
    return 0;
}
