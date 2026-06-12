// linkage2.cpp — solution (the fix is in linkage2_point.h)

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
