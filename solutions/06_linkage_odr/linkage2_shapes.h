// linkage2_shapes.h — unchanged.
#ifndef CPPLINGS_LINKAGE2_SHAPES_H
#define CPPLINGS_LINKAGE2_SHAPES_H

#include "linkage2_point.h"

struct Rect {
    Point lo;
    Point hi;
};

inline int area(const Rect& r) {
    return (r.hi.x - r.lo.x) * (r.hi.y - r.lo.y);
}

inline bool contains(const Rect& r, Point p) {
    return p.x >= r.lo.x && p.x <= r.hi.x &&
           p.y >= r.lo.y && p.y <= r.hi.y;
}

#endif  // CPPLINGS_LINKAGE2_SHAPES_H
