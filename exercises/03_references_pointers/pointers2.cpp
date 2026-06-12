// pointers2.cpp
//
// A raw pointer says nothing about OWNERSHIP — who is responsible for
// freeing the memory, and when? Get it wrong in either direction:
//
//   use-after-free  — someone reads through a pointer after the owner
//                     deleted the object
//   double-free     — two "owners" both delete
//   leak            — no one deletes
//
// All three compile cleanly. The first two are undefined behavior that
// AddressSanitizer catches red-handed, with a stack trace for the bad
// access AND for the free. Run this as-is and read that report carefully —
// learning to read ASan output is a skill worth real money.
//
// Task: fix average() by reordering — compute BEFORE the data dies. The
// real lesson: this whole bug class disappears when ownership is explicit
// (std::unique_ptr, module 09) or when you don't heap-allocate at all
// (a plain std::vector local would do here).

// I AM NOT DONE

#include <cassert>

int* load_samples(int n) {
    int* data = new int[n];          // caller becomes the owner
    for (int i = 0; i < n; ++i) {
        data[i] = (i + 1) * 10;      // 10, 20, 30, ...
    }
    return data;
}

int average(int n) {
    int* samples = load_samples(n);
    delete[] samples;                // ownership discharged... too early
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += samples[i];           // heap-use-after-free
    }
    return sum / n;
}

int main() {
    assert(average(4) == 25);        // (10+20+30+40)/4
    return 0;
}
