// linkage1.cpp        (compiled together with linkage1_tracker.cpp)
//
// This exercise is TWO translation units. The other file,
// linkage1_tracker.cpp, owns a global counter and a function:
//
//     int hit_count = 0;                  // THE definition
//     void record_hit() { ++hit_count; }
//
// This file wants to use both. For the function it's easy: a function
// declaration (a prototype) is obviously not a definition. But for the
// VARIABLE, C++ has a trap C doesn't:
//
//     int hit_count;     // in C: a "tentative definition", merged by the
//                        //       linker with the other one. Fine.
//                        // in C++: A FULL DEFINITION (zero-initialized).
//                        //       Two TUs defining it = duplicate symbol.
//
// So this file must not DEFINE hit_count — it needs a way to say "that
// variable exists, with external linkage; someone else defines it". C++
// has a one-keyword spelling for exactly that.
//
// Note the error you get right now comes from the LINKER (duplicate
// symbol), not the compiler — both TUs compile happily in isolation.
//
// Task: make the two files link, run, and exit 0.
//   - the assert must observe the increments made by the OTHER file
// Constraints:
//   - linkage1_tracker.cpp stays untouched — it rightfully owns the counter
//   - this file must keep referring to that same hit_count: no local
//     stand-in, no renaming, no accessor function

#include <cassert>

int hit_count;        // oops: a second definition
void record_hit();    // fine: declares, doesn't define

int main() {
    record_hit();
    record_hit();
    record_hit();
    assert(hit_count == 3);   // must observe the OTHER file's increments
    return 0;
}

// I AM NOT DONE
