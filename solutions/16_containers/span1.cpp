// span1.cpp — solution

#include <array>
#include <cassert>
#include <cstddef>
#include <span>
#include <vector>

// One non-owning view; vector, std::array, and C array all convert.
int sum(std::span<const int> xs) {
    int total = 0;
    for (int x : xs) total += x;
    return total;
}

int main() {
    std::vector<int> v = {1, 2, 3};
    std::array<int, 4> a = {10, 20, 30, 40};
    int c[] = {5, 5};

    assert(sum(v) == 6);
    assert(sum(a) == 100);
    assert(sum(c) == 10);

    assert(sum(std::span<const int>(a).subspan(1, 2)) == 50);
    return 0;
}
