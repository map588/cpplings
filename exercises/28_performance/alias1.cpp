// alias1.cpp
//
// The optimizer's quietest enemy is ALIASING: the possibility that
// two pointers name the same memory. Look through the compiler's
// eyes:
//
//     void saxpy(float a, const float* x, float* y, int n) {
//         for (int i = 0; i < n; ++i)
//             y[i] = a * x[i] + y[i];
//     }
//
// May it load x[0..7] into one SIMD register and y[0..7] into
// another? Only if x and y don't overlap — and nothing in the types
// says so. Some caller might pass y = x + 1, making every store to
// y[i] change the NEXT iteration's x. So the compiler must assume
// the worst: scalar loads, one at a time, in order. (Compare
// `-Rpass-analysis=loop-vectorize` output some day — "cannot prove
// pointers do not alias" is the classic vectorization killer.)
//
// The promise that unlocks it:
//
//     const float* __restrict__ x, float* __restrict__ y
//
// restrict is C's keyword; C++ compilers all take __restrict__. It
// means: through the LIFETIME of these pointers, the objects they
// touch are touched through THEM ALONE. Now x can't change when y
// is stored — vector registers it is.
//
// Like [[likely]] it's a promise, and like __builtin_unreachable a
// LIE is UB: call restrict-saxpy with overlapping arrays and the
// results are garbage the optimizer built in good faith.
//
// You have been using this contract for years without signing it:
// memcpy's parameters are restrict — overlap is UB. That's the whole
// difference from memmove, which promises overlap-safety and pays a
// direction check for it. ASan knows the contract: part B's
// overlapping memcpy dies with "memcpy-param-overlap". Fix: say
// memmove when you mean overlap.
//
// Task:
//   A) implement saxpy (loop body is in the comment above) with both
//      pointers __restrict__-qualified
//   B) erase_front copies a buffer one slot leftward — onto itself.
//      Overlap. Choose the right mem* call.
// Constraints:
//   - keep the __restrict__ qualifiers on saxpy; its callers here
//     honor the contract
//   - erase_front stays a single mem* call plus nothing else

#include <cassert>
#include <cstring>      // memcpy, memmove

// y[i] = a * x[i] + y[i] — the BLAS classic.
void saxpy(float a, const float* __restrict__ x, float* __restrict__ y,
           int n) {
    (void)a; (void)x; (void)y; (void)n;
    // TODO: the loop from the header comment
}

// Remove buf[0]: slide buf[1..n-1] one slot left. Overlapping ranges!
void erase_front(float* buf, int n) {
    std::memcpy(buf, buf + 1, static_cast<size_t>(n - 1) * sizeof(float));
}

int main() {
    // A) saxpy on separate buffers — the restrict contract, honored.
    const float x[] = {1.0f, 2.0f, 3.0f, 4.0f};
    float y[] = {10.0f, 20.0f, 30.0f, 40.0f};
    saxpy(2.0f, x, y, 4);
    assert(y[0] == 12.0f && y[1] == 24.0f);
    assert(y[2] == 36.0f && y[3] == 48.0f);

    // B) the in-place shift.
    float ring[] = {9.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f};
    volatile int n = 8;                   // runtime n: a real call, not
    erase_front(ring, n);                 // a compile-time expansion
    assert(ring[0] == 1.0f && ring[6] == 7.0f);
    return 0;
}

// I AM NOT DONE
