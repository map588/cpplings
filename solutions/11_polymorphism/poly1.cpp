// poly1.cpp — solution

#include <cassert>
#include <cstddef>

struct PlainPoint {
    double x, y;
    double norm2() const { return x * x + y * y; }
};

struct VirtPoint {
    double x, y;
    virtual double norm2() const { return x * x + y * y; }
    virtual ~VirtPoint() = default;
};

struct ScaledPoint final : VirtPoint {
    double norm2() const override { return 4.0 * (x * x + y * y); }
};

int main() {
    // 1. 16 without, 24 with: the vptr is a real member you pay for.
    static_assert(sizeof(PlainPoint) == 16);
    static_assert(sizeof(VirtPoint) == 24);

    // 2.
    ScaledPoint s{};
    s.x = 1.0; s.y = 2.0;

    VirtPoint v = s;     // a genuine VirtPoint (sliced copy of the base part)
    VirtPoint& r = s;    // still the ScaledPoint underneath

    assert(v.norm2() == 5.0);    // value → static dispatch → base body
    assert(r.norm2() == 20.0);   // reference → vtable → ScaledPoint body
    return 0;
}
