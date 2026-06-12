// vector1.cpp
//
// A vector is {pointer, size, capacity}. push_back beyond capacity:
// allocate bigger buffer (~2x), MOVE every element across, free the old
// buffer. Amortized O(1), occasionally O(n) — and on that occasion,
// every pointer/reference/iterator into the vector now aims at freed
// memory.
//
// The code below grabs a pointer to the first sample, then keeps
// pushing. ASan will show you the heap-use-after-free, with the free
// buried inside vector's reallocation.
//
// Two honest fixes:
//   - reserve(n) up front when you know n: one allocation, pointers
//     stable for the run, no move storms
//   - or don't hold pointers across growth — hold an INDEX; indexes
//     survive reallocation
//
// (How the elements travel during growth — moved or copied — was
// move3's noexcept story. Same event, seen from the other side.)
//
// Task: fix record_baseline-and-friends with reserve(). The capacity
// asserts encode the win.

// I AM NOT DONE

#include <cassert>
#include <vector>

int main() {
    std::vector<double> samples;

    samples.push_back(36.6);            // baseline reading
    const double* baseline = &samples[0];

    for (int i = 0; i < 1000; ++i) {     // a day of telemetry
        samples.push_back(36.6 + i * 0.001);
    }

    // Has the baseline drifted? (Reading through a pointer into a
    // buffer that was freed several reallocations ago...)
    assert(*baseline == 36.6);

    assert(samples.size() == 1001);
    // With reserve done right, the day causes ZERO reallocations:
    // (uncomment once fixed)
    // assert(samples.capacity() == 1001);
    return 0;
}
