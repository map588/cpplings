// rtti1.cpp — solution

#include <cassert>
#include <typeinfo>

struct Shape {
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

struct Circle : Shape {
    double r;
    explicit Circle(double radius) : r(radius) {}
    double area() const override { return 3.0 * r * r; }
};

struct Square : Shape {
    double side;
    explicit Square(double s) : side(s) {}
    double area() const override { return side * side; }
};

double radius_or_zero(const Shape& s) {
    // Pointer form: nullptr when `s` is not a Circle. Checked, no UB.
    if (const Circle* c = dynamic_cast<const Circle*>(&s)) {
        return c->r;
    }
    return 0.0;
}

int main() {
    Circle circle(2.0);
    Square square(5.0);

    assert(radius_or_zero(circle) == 2.0);
    assert(radius_or_zero(square) == 0.0);

    const Shape& s = circle;
    assert((typeid(s) == typeid(Circle)) == true);    // the DYNAMIC type
    assert((typeid(s) == typeid(Shape)) == false);    // not the static one
    return 0;
}
