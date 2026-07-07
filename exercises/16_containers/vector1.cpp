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
// Two honest fixes exist: pre-plan the storage so a known day of data
// never triggers growth (vector's API lets you ask for room up front),
// or stop holding a pointer across growth — an INDEX survives
// reallocation. The asserts below pin down which one today wants.
//
// (How the elements travel during growth — moved or copied — was
// move3's noexcept story. Same event, seen from the other side.)
//
// Task: make the program run clean under ASan with every assert passing.
//   - the baseline pointer stays valid for the whole run
//   - the whole day of telemetry costs exactly ONE allocation — that is
//     what the capacity assert encodes
// Constraints:
//   - `baseline` stays a pointer, taken exactly where it is now
//   - don't change or remove any assert
//   - the fix is one line

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
    // With the storage plan right, the day causes ZERO reallocations:
    assert(samples.capacity() == 1001);
    return 0;
}

// I AM NOT DONE
