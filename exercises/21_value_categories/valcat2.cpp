// valcat2.cpp
//
// A function's RETURN TYPE decides the value category of every call to
// it:
//
//     int  f();     // f() is a PRVALUE — pure value, not assignable
//     int& f();     // f() is an LVALUE — you can write f() = 5;
//     int&& f();    // f() is an XVALUE — movable-from
//
// `T&`-returning functions are how operator[] and .front() let you
// write `v[i] = x;` — the call IS the element, aliased. That's the
// power; the price is a lifetime promise (valcat4).
//
// brightest_pixel() below returns by VALUE, so the call is a prvalue —
// and assigning to a prvalue ("expression is not assignable") is
// today's compile error. The caller wants to DIM that pixel in place,
// writing through the call itself.
//
// Task: make main compile and pass exactly as written.
//   - the two assignments through brightest_pixel(image) must modify
//     the caller's vector (the asserts check the contents)
// Constraints:
//   - do not change main
//   - brightest_pixel still returns "the pixel" — not an index, not a
//     pointer
//   - mind the lifetime promise you're making: the object behind what
//     you return must outlive the call (here the caller's vector does —
//     valcat4 shows the illegitimate twin)

#include <cassert>
#include <vector>

// Returns the brightest pixel of a non-empty image... a copy of it.
int brightest_pixel(std::vector<int>& image) {
    std::size_t best = 0;
    for (std::size_t i = 1; i < image.size(); ++i) {
        if (image[i] > image[best]) best = i;
    }
    return image[best];
}

int main() {
    std::vector<int> image = {18, 240, 77, 240, 3};

    assert(brightest_pixel(image) == 240);

    brightest_pixel(image) = 120;          // dim the (first) brightest
    assert(image == (std::vector<int>{18, 120, 77, 240, 3}));

    brightest_pixel(image) /= 2;           // compound-assign through it too
    assert(image == (std::vector<int>{18, 120, 77, 120, 3}));
    return 0;
}

// I AM NOT DONE
