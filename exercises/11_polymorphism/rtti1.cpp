// rtti1.cpp
//
// Virtual dispatch answers "do the right thing for whatever you are".
// Sometimes code asks the cruder question: "ARE you a Circle?" — and
// wants the Circle-only members if so. That is a DOWNCAST, base to
// derived, and C++ gives you two ways to do it:
//
//   static_cast<const Circle&>(shape)
//       No check. You are telling the compiler "trust me, it is one".
//       If it isn't, that is undefined behavior: the code reads a
//       Circle's members out of an object that has none. Zero
//       instructions, zero protection.
//
//   dynamic_cast<const Circle*>(&shape)
//       CHECKED at runtime. Every polymorphic object carries run-time
//       type information (RTTI) next to its vtable; dynamic_cast looks
//       it up and answers honestly. The pointer form yields nullptr on
//       a mismatch; the reference form cannot be null, so it throws
//       std::bad_cast instead (exceptions: module 20).
//
// The honesty has a price — a search through type records at runtime,
// where static_cast costs nothing — and it only works on POLYMORPHIC
// types (at least one virtual function; no vtable, no RTTI). The same
// records feed `typeid(expr)`, which through a reference to a
// polymorphic type names the object's DYNAMIC type.
//
// Guidance worth keeping: a downcast is a question the design failed to
// answer. First choice is a virtual function, so nobody has to ask. For
// a closed set of types, std::variant (module 19) answers at compile
// time. dynamic_cast is the honest fallback — and some embedded builds
// forbid even that (-fno-rtti).
//
// radius_or_zero() below downcasts on trust. Handed a Square it reads
// the Square's `side` as if it were a radius (the two doubles happen to
// sit in the same place). UBSan may name the bad cast; the assert
// catches the wrong number either way.
//
// Task: make radius_or_zero ask before it reaches, then settle the quiz.
//   - a Circle yields its radius; anything else yields 0.0 — with no
//     undefined behavior on the way
//   - replace each TODO_BOOL with the truth about typeid
// Constraints:
//   - radius_or_zero keeps its signature and must still work for ANY
//     Shape, including types that do not exist yet
//   - don't add a virtual function to Shape for this — today's lesson
//     is the cast (and the guidance above is why you'd rather not)
//   - quiz: change only the TODO_BOOL values

#include <cassert>
#include <typeinfo>

constexpr bool TODO_BOOL = false;

struct Shape {
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

struct Circle : Shape {
    double r;
    explicit Circle(double radius) : r(radius) {}
    double area() const override { return 3.0 * r * r; }   // pi ≈ 3 today
};

struct Square : Shape {
    double side;
    explicit Square(double s) : side(s) {}
    double area() const override { return side * side; }
};

// The radius if `s` is a Circle, otherwise 0.0.
double radius_or_zero(const Shape& s) {
    const Circle& c = static_cast<const Circle&>(s);   // "trust me"
    return c.r;
}

int main() {
    Circle circle(2.0);
    Square square(5.0);

    assert(radius_or_zero(circle) == 2.0);
    assert(radius_or_zero(square) == 0.0);    // a Square has no radius

    // Quiz. `s` has STATIC type Shape — what does typeid report?
    const Shape& s = circle;
    assert((typeid(s) == typeid(Circle)) == TODO_BOOL);
    assert((typeid(s) == typeid(Shape)) == TODO_BOOL);
    return 0;
}

// I AM NOT DONE
