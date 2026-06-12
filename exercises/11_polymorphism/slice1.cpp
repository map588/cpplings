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
// Polymorphism needs INDIRECTION. References for parameters; for owning
// containers, std::vector<std::unique_ptr<Shape>> (module 10 — this is
// the use case that module was building toward).
//
// Task: fix describe() to take a reference, and rebuild `shapes` as a
// vector of unique_ptr<Shape>. (The asserts stay — make them true.)

// I AM NOT DONE

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

    assert(shapes[0]->name() == "circle");   // (pointer syntax: the fixed
    assert(shapes[1]->name() == "square");   //  version holds unique_ptrs)
    return 0;
}
