// ops2.cpp — solution

#include <cassert>
#include <compare>

struct Version {
    int major;
    int minor;
    int patch;

    // All six comparisons, member-wise in declaration order.
    auto operator<=>(const Version&) const = default;
};

int main() {
    Version v1_2_3{1, 2, 3};
    Version v1_10_0{1, 10, 0};
    Version dup{1, 2, 3};

    assert(v1_2_3 < v1_10_0);
    assert(v1_10_0 > v1_2_3);
    assert(v1_2_3 == dup);
    assert(v1_2_3 != v1_10_0);
    assert(v1_2_3 <= dup);
    assert(v1_10_0 >= v1_2_3);
    return 0;
}
