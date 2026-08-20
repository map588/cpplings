// simd3.cpp
//
// The third road to SIMD: INTRINSICS — C functions that map ~1:1 to
// machine instructions. Zero portability (this file needs two
// complete implementations!), total control. Real codebases wrap
// them exactly like this: one algorithm, one #if ladder per
// architecture. You write the lane arithmetic; the names differ,
// the thinking is simd1/simd2's.
//
//   ARM NEON (<arm_neon.h>)              x86 SSE (<immintrin.h>)
//   float32x4_t                          __m128
//   vdupq_n_f32(0)     broadcast 0       _mm_setzero_ps()
//   vld1q_f32(p)       load 4 floats     _mm_loadu_ps(p)
//   vmulq_f32 / vaddq_f32                _mm_mul_ps / _mm_add_ps
//   vmlaq_f32(acc,a,b)  acc + a*b        (mul then add on SSE)
//   vaddvq_f32(v)      sum all lanes     (shuffle+add, simd2 style)
//
// The provided code handles the VECTOR BODY: n/4 rounds of 4 lanes.
// It is correct as far as it goes — run the asserts and see which n
// it fails for. SIMD's eternal bookkeeping problem: n is not a
// multiple of the lane count. The last n%4 elements need a SCALAR
// TAIL LOOP after the vector body. Forgetting it is THE classic
// intrinsics bug — silently correct on benchmark sizes, wrong on
// real ones.
//
// Task: add the scalar tail to both implementations.
//   - dot(x, y, n) is exact for every n, not just multiples of 4
// Constraints:
//   - keep the vector body vectorized — the tail handles ONLY the
//     last n % 4 elements
//   - (values here are small integers in float form, so sums are
//     exact — no epsilon games)

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
    // TODO: the scalar tail — elements i .. n-1
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
    // TODO: the scalar tail — elements i .. n-1
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

// I AM NOT DONE
