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
//     template <typename T, std::size_t N>
//     constexpr std::size_t count(T (&)[N]) { return N; }
//                              // ^ a REFERENCE to an array of exactly N
//
// A reference-to-array does NOT decay — N rides into the template as a
// compile-time value, deduced from the argument. (This exact gadget is
// std::size() since C++17 — use that in real code; write it yourself
// once, here.)
//
// Task: replace the pointer version with the template. The static_assert
// proves the count is now a compile-time constant.

#include <cassert>
#include <cstddef>

std::size_t count(int* xs) {
    return sizeof(xs) / sizeof(xs[0]);     // sizeof(int*) / sizeof(int)
}

int main() {
    int readings[] = {62, 58, 71, 64};

    assert(count(readings) == 4);          // says 2. Pointer decay strikes.

    // After templatizing, the result is usable at compile time:
    // (uncomment once your count() is constexpr)
    // static_assert(count(readings) == 4);
    // int copy[count(readings)] = {};      // even as an array bound
    // assert(sizeof(copy) == sizeof(readings));
    return 0;
}

// I AM NOT DONE
