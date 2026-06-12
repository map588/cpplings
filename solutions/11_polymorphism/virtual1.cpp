// virtual1.cpp — solution

#include <cassert>

struct Shape {
    virtual double area() const { return 0.0; }
    virtual ~Shape() = default;    // polymorphic base → virtual dtor
};                                 // (virtdtor1 explains the whole story)

struct Rectangle : Shape {
    double w, h;
    Rectangle(double w, double h) : w(w), h(h) {}
    double area() const override { return w * h; }
};

struct Triangle : Shape {
    double base, height;
    Triangle(double b, double h) : base(b), height(h) {}
    double area() const override { return base * height / 2; }
};

double total_area(const Shape& a, const Shape& b) {
    return a.area() + b.area();    // dispatched on the actual objects
}

int main() {
    Rectangle r(3, 4);
    Triangle t(6, 5);

    assert(r.area() == 12.0);
    assert(t.area() == 15.0);

    assert(total_area(r, t) == 27.0);
    return 0;
}
