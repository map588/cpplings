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
// today's compile error. The caller wants to DIM that pixel in place.
//
// Task: make brightest_pixel return a reference to the element inside
// the caller's vector. (Returning a reference INTO a parameter is the
// legitimate version of valcat4's crime: the caller owns the vector,
// which outlives the call.)

// I AM NOT DONE

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
