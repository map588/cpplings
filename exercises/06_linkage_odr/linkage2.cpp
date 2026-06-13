// linkage2.cpp        (uses linkage2_point.h and linkage2_shapes.h)
//
// #include is TEXTUAL PASTE — the preprocessor splices the file in,
// nothing smarter. So when a header reaches a translation unit twice
// (here: once directly, once through linkage2_shapes.h), every definition
// in it appears twice in the same TU, and the compiler reports a
// redefinition.
//
// The fix lives in the HEADER, and there are two spellings:
//
//   #pragma once                  // top of file. Non-standard, but
//                                 // supported by every compiler you'll
//                                 // ever meet, and impossible to typo.
//
//   #ifndef LINKAGE2_POINT_H      // the standard-blessed classic. The
//   #define LINKAGE2_POINT_H      // macro name must be unique per header
//   ...                           // — copy-pasting a guard and forgetting
//   #endif                        // to rename it causes WONDERFUL bugs
//                                 // (the second header silently vanishes).
//
// Why was it legal to define `struct Point` in a header at all, when
// defining a global variable there breaks (see linkage5)? Because the ODR
// carves out types: a class may be defined once PER TU, as long as all
// TUs agree token-for-token. Twice in the SAME TU is still an error.
//
// Task: fix linkage2_point.h so this file compiles. (linkage2_shapes.h
// already has a guard — peek at it for the pattern.)

#include "linkage2_point.h"
#include "linkage2_shapes.h"

#include <cassert>

int main() {
    Rect r{{0, 0}, {4, 3}};
    assert(area(r) == 12);

    Point p{2, 2};
    assert(contains(r, p));
    return 0;
}

// I AM NOT DONE
