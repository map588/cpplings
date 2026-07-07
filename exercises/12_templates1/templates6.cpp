// templates6.cpp
//
// Templates take VALUES too — non-type template parameters (NTTPs).
// You've used them all along: std::array<int, 4>; the 4 is an NTTP.
//
// The bug below is one of C's most venerable: count() takes `int* xs`,
// and inside the function sizeof(xs) is the size of A POINTER (8), not
// the array — the array DECAYED at the call (module 01). 8/4 = 2
// elements, says the function, about a 4-element array.
//
// The C++ cure combines two features:
//
//   - a parameter that is a REFERENCE TO AN ARRAY does not decay. The
//     (admittedly baroque) declarator for "reference to an array of
//     exactly 4 ints" is:
//
//         int (&xs)[4]
//
//   - an NTTP can stand where the 4 is — and be DEDUCED from the
//     argument, so the length rides into the template as a compile-time
//     value, one instantiation per array size.
//
// (This exact gadget is std::size() since C++17 — use that in real code.
// Here, build it yourself once.)
//
// Task: replace the pointer version of count() with the template gadget.
//   - the existing assert passes: count(readings) == 4
//   - the result must be usable at compile time — enable the commented
//     block at the bottom of main; it must then compile and pass as is
// Constraints:
//   - count() must deduce the length itself: no std::size inside (that
//     would be buying the gadget you're here to build)
//   - it must work for any element type and any length, not just int[4]
//   - don't change the lines in the commented block — just enable them

#include <cassert>
#include <cstddef>

std::size_t count(int* xs) {
    return sizeof(xs) / sizeof(xs[0]);     // sizeof(int*) / sizeof(int)
}

int main() {
    int readings[] = {62, 58, 71, 64};

    assert(count(readings) == 4);          // says 2. Pointer decay strikes.

    // Compile-time proof — see the Task block:
    // static_assert(count(readings) == 4);
    // int copy[count(readings)] = {};      // even as an array bound
    // assert(sizeof(copy) == sizeof(readings));
    return 0;
}

// I AM NOT DONE
