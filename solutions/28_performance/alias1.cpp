// alias1.cpp — SOLUTION
//
// saxpy: the plain loop — __restrict__ does the performance work by
// LETTING the vectorizer act; the source stays boring. erase_front:
// overlap means memmove. memcpy's parameters are restrict-qualified,
// so overlapping them is UB (and ASan's memcpy-param-overlap check
// catches it red-handed).

#include <cassert>
#include <cstring>      // memcpy, memmove

// y[i] = a * x[i] + y[i] — the BLAS classic.
void saxpy(float a, const float* __restrict__ x, float* __restrict__ y,
           int n) {
    for (int i = 0; i < n; ++i) {
        y[i] = a * x[i] + y[i];
    }
}

// Remove buf[0]: slide buf[1..n-1] one slot left. Overlapping ranges!
void erase_front(float* buf, int n) {
    std::memmove(buf, buf + 1, static_cast<size_t>(n - 1) * sizeof(float));
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
