// templates3.cpp — solution

#include <cassert>
#include <vector>

template <typename T>
T sum_every_other(const std::vector<T>& v) {
    T total{};
    // The dutiful fix — tell the compiler it's a type:
    //     typename std::vector<T>::const_iterator it = v.begin();
    // The modern fix — never name the type at all:
    auto it = v.begin();
    while (it != v.end()) {
        total += *it;
        if (it + 1 == v.end()) break;
        it += 2;
    }
    return total;
}

int main() {
    std::vector<int> ints = {1, 2, 3, 4, 5};
    assert(sum_every_other(ints) == 9);
    std::vector<double> reals = {0.5, 9.0, 1.5};
    assert(sum_every_other(reals) == 2.0);
    return 0;
}
