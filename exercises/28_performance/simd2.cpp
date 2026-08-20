// simd2.cpp
//
// simd1 moved lanes in lockstep. The other half of SIMD thinking is
// moving data BETWEEN lanes: shuffles, and the reductions built from
// them.
//
//     __builtin_shufflevector(a, b, i0, i1, i2, i3)
//
// builds a new vector by INDEX: 0..3 pick lanes of a, 4..7 pick
// lanes of b. (clang always had it; gcc since 12.) Examples:
//
//     __builtin_shufflevector(a, a, 3, 2, 1, 0)   // reverse a
//     __builtin_shufflevector(a, b, 0, 4, 1, 5)   // interleave heads
//
// The classic reduction — summing all lanes — is built from shuffles
// too. Lane-at-a-time (a[0]+a[1]+a[2]+a[3]) serializes; log2(n)
// shuffle+add rounds keep the vector unit busy:
//
//     step 1: v = v + shuffle(v, v, 2, 3, ?, ?)   // hi half onto lo
//             → lanes 0,1 now hold a0+a2, a1+a3
//     step 2: v = v + shuffle(v, v, 1, ?, ?, ?)   // lane 1 onto 0
//             → lane 0 holds the total
//
// (`?` = a lane whose value you no longer care about — any index is
// fine; convention picks ones already there. With 4 lanes the win
// is small; with AVX-512's 16 it is the difference between 15 adds
// and 4.)
//
// Task:
//   - reverse4: flip lane order (one shuffle)
//   - hsum4:    sum the lanes via the two-round scheme above —
//               lane-at-a-time addition is the one banned move
//   - dot4:     dot product of a and b, built from what you have
// Constraints:
//   - hsum4: exactly two additions
//   - dot4: one multiply, then reuse hsum4

#include <cassert>

using f32x4 = float __attribute__((vector_size(16)));   // 4 × float

f32x4 reverse4(f32x4 a) {
    return a;                     // TODO: one shuffle
}

float hsum4(f32x4 v) {
    return v[0];                  // TODO: two shuffle+add rounds, then lane 0
}

float dot4(f32x4 a, f32x4 b) {
    (void)b;
    return a[0];                  // TODO: multiply, then hsum4
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

// I AM NOT DONE
