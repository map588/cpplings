// algos1.cpp — solution

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

double total_weight(const std::vector<double>& weights) {
    return std::accumulate(weights.begin(), weights.end(), 0.0);
    //                                            int 0 here ^ would
    //                                            truncate every step
}

double heaviest(const std::vector<double>& weights) {
    auto it = std::max_element(weights.begin(), weights.end());
    return it == weights.end() ? 0.0 : *it;     // empty-range guard
}

bool carries(const std::vector<double>& weights, double exact) {
    return std::find(weights.begin(), weights.end(), exact)
           != weights.end();
}

int main() {
    std::vector<double> cargo = {0.5, 2.25, 1.0, 3.75};

    assert(total_weight(cargo) == 7.5);
    assert(heaviest(cargo) == 3.75);
    assert(carries(cargo, 2.25));
    assert(!carries(cargo, 9.99));
    return 0;
}
