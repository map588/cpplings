// poly1.cpp  — QUIZ for module 11: what does `virtual` cost?
//
// The price list:
//
//   per OBJECT:  one hidden vptr — sizeof grows by 8 (and alignment may
//                pad further). A million small objects notice.
//   per CLASS:   one vtable in the binary (usually irrelevant).
//   per CALL:    pointer chase + indirect call. The real cost is usually
//                the INLINING the optimizer can no longer do.
//   dispatch only through refs/pointers: a call on a VALUE is resolved
//                statically — even though the function is virtual.
//
// `final` (C++11) is the escape valve: a final class (or method) can't
// be derived from (or overridden), so the compiler may DEVIRTUALIZE
// calls it can see through — back to direct, inlinable calls.
//
// Task: fill in the TODOs. sizeof answers assume a typical 64-bit
// platform (pointers are 8 bytes) — which is what this course runs on.

#include <cassert>
#include <cstddef>

constexpr std::size_t TODO = 9999;

struct PlainPoint {
    double x, y;
    double norm2() const { return x * x + y * y; }
};

struct VirtPoint {
    double x, y;
    virtual double norm2() const { return x * x + y * y; }
    virtual ~VirtPoint() = default;
};

// `final`: nothing can derive from this, so calls through a
// ScaledPoint& can be devirtualized.
struct ScaledPoint final : VirtPoint {
    double norm2() const override { return 4.0 * (x * x + y * y); }
};

// struct Extra : ScaledPoint {};   // uncomment: refuses to compile

int main() {
    // 1. The vptr in numbers:
    static_assert(sizeof(PlainPoint) == TODO);   // two doubles
    static_assert(sizeof(VirtPoint) == TODO);    // two doubles + vptr

    // 2. Virtual dispatch needs indirection. `v` is a VALUE of static
    //    type VirtPoint — what does each call run?
    ScaledPoint s{};
    s.x = 1.0; s.y = 2.0;                        // norm2 base: 5.0

    VirtPoint v = s;                             // sliced copy (slice1!)
    VirtPoint& r = s;                            // reference to the real s

    assert(v.norm2() == static_cast<double>(TODO));   // value: static dispatch
    assert(r.norm2() == static_cast<double>(TODO));   // ref: virtual dispatch
    return 0;
}

// I AM NOT DONE
