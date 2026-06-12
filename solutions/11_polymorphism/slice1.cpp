// slice1.cpp — solution

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

std::string describe(const Shape& s) {       // reference: no copy, no slice
    return "a " + s.name();
}

int main() {
    Circle c;
    assert(describe(c) == "a circle");

    // Owning, polymorphic container: pointers, not values.
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>());
    shapes.push_back(std::make_unique<Square>());

    assert(shapes[0]->name() == "circle");
    assert(shapes[1]->name() == "square");
    return 0;
}
