// simd1.cpp
//
// Your CPU has registers that hold FOUR floats (or more) and ALUs
// that operate on all lanes in one instruction: SIMD — Single
// Instruction, Multiple Data. NEON on ARM, SSE/AVX on x86. Three
// roads lead there:
//
//   1. auto-vectorization — write clean loops at -O2/-O3 and let the
//      compiler do it (what alias1's restrict was unblocking)
//   2. VECTOR EXTENSIONS — this exercise: portable SIMD types that
//      clang and gcc compile to whatever the target CPU has
//   3. intrinsics — per-architecture, maximum control (simd3)
//
// A vector type is declared by widening a scalar:
//
//     using f32x4 = float __attribute__((vector_size(16)));
//                                  // 16 bytes = 4 lanes of float
//
// and then it behaves like a float that is secretly four floats:
//
//     f32x4 a = {1, 2, 3, 4};      // braces fill the lanes
//     f32x4 b = a + a;             // ONE add instruction, 4 lanes
//     b = b * 3.0f;                // scalars broadcast to every lane
//     b[2]                         // lanes index like an array
//
// The mental shift is the entire skill: stop thinking "loop over
// elements", start thinking "one operation on all lanes". No
// branches per element, no early exits — arithmetic on columns of
// data. (Same model as GPU shaders and NumPy.)
//
// These are the same lane-wise formulas graphics and audio code run
// all day. This file is built -O2, sanitizers off — inspect the
// assembly some day: fmla.4s on ARM, mulps/addps on x86.
//
// Task: implement the three lane-wise functions as vector
// EXPRESSIONS — no loops, no per-lane indexing.
//   - axpy4:  a*x + y             (alias1's saxpy, four lanes at once)
//   - lerp4:  start + (stop - start) * t     (the blend everywhere in
//                                              graphics)
//   - halfway4: midpoint of two vectors
// Constraints:
//   - no `for`, no `[i]` on the left-hand side — lanes move together
//   - scalar constants are fine: broadcasting is the idiom

#include <cassert>

using f32x4 = float __attribute__((vector_size(16)));   // 4 × float

f32x4 axpy4(float a, f32x4 x, f32x4 y) {
    (void)a; (void)x;
    return y;                     // TODO: a*x + y, all lanes at once
}

f32x4 lerp4(f32x4 start, f32x4 stop, float t) {
    (void)stop; (void)t;
    return start;                 // TODO: start + (stop - start) * t
}

f32x4 halfway4(f32x4 a, f32x4 b) {
    (void)b;
    return a;                     // TODO: the midpoint of each lane pair
}

int main() {
    const f32x4 x = {1.0f, 2.0f, 3.0f, 4.0f};
    const f32x4 y = {10.0f, 20.0f, 30.0f, 40.0f};

    f32x4 r = axpy4(2.0f, x, y);
    assert(r[0] == 12.0f && r[1] == 24.0f && r[2] == 36.0f && r[3] == 48.0f);

    r = lerp4(x, y, 0.5f);        // halfway blend
    assert(r[0] == 5.5f && r[1] == 11.0f && r[2] == 16.5f && r[3] == 22.0f);

    r = lerp4(x, y, 0.0f);        // t = 0 → exactly start
    assert(r[0] == x[0] && r[3] == x[3]);

    r = halfway4(x, y);
    assert(r[0] == 5.5f && r[1] == 11.0f && r[2] == 16.5f && r[3] == 22.0f);
    return 0;
}

// I AM NOT DONE
