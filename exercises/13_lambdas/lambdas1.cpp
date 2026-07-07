// lambdas1.cpp
//
// Meet your first lambda by building its longhand twin first.
//
// This is a FUNCTOR — a class whose objects are callable:
//
//     struct ScaleBy {
//         int factor;                                  // carried state
//         int operator()(int x) const { return x * factor; }
//     };
//     ScaleBy triple{3};
//     triple(7);                                       // 21
//
// And a lambda is the SAME MACHINE written inline. Here's one for a
// different job — adding a captured offset:
//
//     int bonus = 5;
//     auto boost = [bonus](int x) { return x + bonus; };    // boost(2) == 7
//
// Anatomy:  [bonus]    capture list → the members (bonus copied in)
//           (int x)    the parameter list of operator()
//           { ... }    body; return type deduced (write `-> int` to force)
//
// The compiler generates the struct, names it something unpronounceable,
// and hands you the object. `auto` is how you hold it — you can't spell
// the type (lambdas6 has fun with that).
//
// Task: in times_ten, write `scale` as the lambda twin of ScaleBy{factor}.
//   - both asserts pass
// Constraints:
//   - `scale` must capture `factor` — no hard-coded 10 in the body
//   - don't use ScaleBy inside times_ten; don't change scaled_sum or
//     ScaleBy
//   - keep the `return scale(x);` line as it is

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
    auto scale = nullptr;    // TODO: the lambda twin of ScaleBy{factor}
    return scale(x);
}

int main() {
    assert(scaled_sum(2, 3, 4) == 20);
    assert(times_ten(7) == 70);
    return 0;
}

// I AM NOT DONE
