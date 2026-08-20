// simd1.cpp — SOLUTION
//
// Each function is one arithmetic expression — the vector type turns
// lane iteration into plain algebra. Scalars (a, t, 0.5f) broadcast
// to every lane automatically.

#include <cassert>

using f32x4 = float __attribute__((vector_size(16)));   // 4 × float

f32x4 axpy4(float a, f32x4 x, f32x4 y) {
    return a * x + y;
}

f32x4 lerp4(f32x4 start, f32x4 stop, float t) {
    return start + (stop - start) * t;
}

f32x4 halfway4(f32x4 a, f32x4 b) {
    return (a + b) * 0.5f;
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
