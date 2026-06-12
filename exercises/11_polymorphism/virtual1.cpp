// virtual1.cpp
//
// Without `virtual`, a call is resolved at COMPILE TIME from the static
// type of the expression. total_area() sees `const Shape&` — so it calls
// Shape::area(), no matter what object is actually standing there.
//
// Mark area() virtual and the call becomes RUNTIME dispatch: look at the
// actual object's vtable, run the most-derived override. The derived
// side should then say `override` (C++11) — it costs nothing and turns
// signature typos into compile errors (next exercise shows the disease
// it prevents).
//
// Note the signatures: area() must be const (these are read-only
// queries through const&, module 02), and the derived signature must
// match the base EXACTLY — const included — or it isn't an override.
//
// Task: make area() dispatch dynamically.

// I AM NOT DONE

#include <cassert>

struct Shape {
    double area() const { return 0.0; }       // "abstract"... allegedly
};

struct Rectangle : Shape {
    double w, h;
    Rectangle(double w, double h) : w(w), h(h) {}
    double area() const { return w * h; }
};

struct Triangle : Shape {
    double base, height;
    Triangle(double b, double h) : base(b), height(h) {}
    double area() const { return base * height / 2; }
};

double total_area(const Shape& a, const Shape& b) {
    return a.area() + b.area();    // static type: Shape. Hmm.
}

int main() {
    Rectangle r(3, 4);
    Triangle t(6, 5);

    assert(r.area() == 12.0);                  // direct calls were fine
    assert(t.area() == 15.0);

    assert(total_area(r, t) == 27.0);          // through Shape&: 0 + 0...
    return 0;
}
