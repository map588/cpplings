// lambdas1.cpp — solution

#include <cassert>

struct ScaleBy {
    int factor;
    int operator()(int x) const { return x * factor; }
};

int scaled_sum(int a, int b, int factor) {
    ScaleBy scale{factor};
    return scale(a) + scale(b);
}

int times_ten(int x) {
    int factor = 10;
    // [factor] copies factor into the closure — the `int factor;` member
    // of the generated struct, in ScaleBy terms.
    auto scale = [factor](int v) { return v * factor; };
    return scale(x);
}

int main() {
    assert(scaled_sum(2, 3, 4) == 20);
    assert(times_ten(7) == 70);
    return 0;
}
