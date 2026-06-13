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
// And this is the SAME THING, written as a lambda:
//
//     auto triple = [factor](int x) { return x * factor; };
//
// Anatomy:  [factor]   capture list → the members (factor copied in)
//           (int x)    parameters of operator()
//           { ... }    body; return type deduced (write `-> int` to force)
//
// The compiler generates the struct, names it something unpronounceable,
// and hands you the object. `auto` is how you hold it — you can't spell
// the type (lambdas6 has fun with that).
//
// Task: scaled_sum uses the functor. Write `scale` as a lambda doing the
// same job, and make times_ten use it.

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
