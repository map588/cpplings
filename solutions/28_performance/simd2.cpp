// simd2.cpp — SOLUTION
//
// reverse4 is one shuffle by index. hsum4 folds the high half onto
// the low half, then lane 1 onto lane 0 — log2(4) = 2 vector adds.
// dot4 is the multiply the vector unit was built for, then the
// reduction.

#include <cassert>

using f32x4 = float __attribute__((vector_size(16)));   // 4 × float

f32x4 reverse4(f32x4 a) {
    return __builtin_shufflevector(a, a, 3, 2, 1, 0);
}

float hsum4(f32x4 v) {
    // Fold the high pair onto the low pair...
    v = v + __builtin_shufflevector(v, v, 2, 3, 2, 3);
    // ...then lane 1 onto lane 0.
    v = v + __builtin_shufflevector(v, v, 1, 1, 1, 1);
    return v[0];
}

float dot4(f32x4 a, f32x4 b) {
    return hsum4(a * b);
}

int main() {
    const f32x4 a = {1.0f, 2.0f, 3.0f, 4.0f};
    const f32x4 b = {10.0f, 100.0f, 1000.0f, 10000.0f};

    f32x4 r = reverse4(a);
    assert(r[0] == 4.0f && r[1] == 3.0f && r[2] == 2.0f && r[3] == 1.0f);

    assert(hsum4(a) == 10.0f);
    assert(hsum4(b) == 11110.0f);

    assert(dot4(a, b) == 43210.0f);   // 10 + 200 + 3000 + 40000
    return 0;
}
