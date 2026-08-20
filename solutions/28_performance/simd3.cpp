// simd3.cpp — SOLUTION
//
// The tail is three lines and identical on both architectures — a
// plain scalar loop from wherever the vector body stopped. i already
// holds that position; that's why the body declared it outside the
// loop.

#include <cassert>

#if defined(__aarch64__)
#include <arm_neon.h>

float dot(const float* x, const float* y, int n) {
    float32x4_t acc = vdupq_n_f32(0.0f);
    int i = 0;
    for (; i + 4 <= n; i += 4) {
        acc = vmlaq_f32(acc, vld1q_f32(x + i), vld1q_f32(y + i));
    }
    float total = vaddvq_f32(acc);        // horizontal sum, one insn
    for (; i < n; ++i) {
        total += x[i] * y[i];             // the last n % 4 elements
    }
    return total;
}

#elif defined(__x86_64__) || defined(_M_X64)
#include <immintrin.h>

float dot(const float* x, const float* y, int n) {
    __m128 acc = _mm_setzero_ps();
    int i = 0;
    for (; i + 4 <= n; i += 4) {
        acc = _mm_add_ps(acc, _mm_mul_ps(_mm_loadu_ps(x + i),
                                         _mm_loadu_ps(y + i)));
    }
    // Horizontal sum, simd2's two-round scheme in SSE spelling:
    __m128 hi = _mm_movehl_ps(acc, acc);           // lanes 2,3 → 0,1
    __m128 s  = _mm_add_ps(acc, hi);
    s = _mm_add_ss(s, _mm_shuffle_ps(s, s, 0x55)); // lane 1 → lane 0
    float total = _mm_cvtss_f32(s);
    for (; i < n; ++i) {
        total += x[i] * y[i];             // the last n % 4 elements
    }
    return total;
}

#else
#error "simd3 has NEON and SSE paths only (exercises.toml skips other CPUs)"
#endif

int main() {
    const float x[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f};
    const float y[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f};

    assert(dot(x, y, 4) == 30.0f);    // benchmark-friendly n: works already
    assert(dot(x, y, 7) == 140.0f);   // real-world n: needs the tail
    assert(dot(x, y, 1) == 1.0f);     // all tail, no vector body
    assert(dot(x, y, 0) == 0.0f);
    return 0;
}
