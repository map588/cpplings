// ub2.cpp — solution

#include <cassert>
#include <stdexcept>
#include <vector>

int sum_all(const std::vector<int>& v) {
    int total = 0;
    for (std::size_t i = 0; i < v.size(); ++i) {   // < not <=
        total += v[i];
    }
    return total;
}

int lookup(const std::vector<int>& v, std::size_t user_index) {
    return v.at(user_index);       // untrusted index → checked access
}

int main() {
    std::vector<int> data = {10, 20, 30};

    assert(sum_all(data) == 60);

    assert(lookup(data, 1) == 20);

    bool caught = false;
    try {
        lookup(data, 9999);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
    return 0;
}
