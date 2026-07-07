// slice1.cpp
//
// OBJECT SLICING: copy a Derived into a Base-typed slot and only the
// Base subobject survives. The derived members are cut off; the vptr is
// the Base one, so even virtual calls run Base versions. The object
// isn't "a Triangle viewed as a Shape" — it's a genuine Shape copy with
// the Triangle sheared away.
//
// The two habitats of this bug:
//
//   void draw(Shape s);              // by-VALUE parameter: slices
//   std::vector<Shape> shapes;       // container of VALUES: slices on
//   shapes.push_back(triangle);      // the way in
//
// Polymorphism needs INDIRECTION. Parameters that don't copy the object;
// container slots that OWN a shape without BEING a shape. You already
// built both tools — module 02's for parameters, and module 10's, whose
// whole reason to exist is exactly this container.
//
// Task: stop the slicing in both habitats.
//   - all asserts pass exactly as written — note the container asserts
//     use ->, so the fixed `shapes` must hold something pointer-like
//   - no leaks: the container owns its shapes (ASan is watching)
// Constraints:
//   - Shape, Circle, Square stay as they are
//   - describe() must still be callable with a Circle directly — no
//     casts or & at the call site
//   - don't change any assert

#include <cassert>
#include <memory>
#include <string>
#include <vector>

struct Shape {
    virtual std::string name() const { return "shape"; }
    virtual ~Shape() = default;
};

struct Circle : Shape {
    std::string name() const override { return "circle"; }
};

struct Square : Shape {
    std::string name() const override { return "square"; }
};

std::string describe(Shape s) {              // by value: slice
    return "a " + s.name();
}

int main() {
    Circle c;
    assert(describe(c) == "a circle");       // "a shape" — c was sliced

    std::vector<Shape> shapes;               // value slots: slicers
    shapes.push_back(Circle{});
    shapes.push_back(Square{});

    assert(shapes[0]->name() == "circle");   // (pointer syntax — a clue about
    assert(shapes[1]->name() == "square");   //  what the fixed vector holds)
    return 0;
}

// I AM NOT DONE
