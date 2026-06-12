// variant2.cpp — solution

#include <cassert>
#include <variant>
#include <vector>

struct Circle   { double r; };
struct Rect     { double w, h; };
struct Triangle { double base, height; };

using Shape = std::variant<Circle, Rect, Triangle>;

template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

double area(const Shape& s) {
    return std::visit(overloaded{
        [](const Circle& c) { return 3.14159 * c.r * c.r; },
        [](const Rect& r) { return r.w * r.h; },
        [](const Triangle& t) { return t.base * t.height / 2; },
    }, s);
}

int main() {
    std::vector<Shape> shapes = {Circle{1.0}, Rect{3, 4}, Triangle{6, 5}};

    assert(area(shapes[0]) > 3.14 && area(shapes[0]) < 3.15);
    assert(area(shapes[1]) == 12.0);
    assert(area(shapes[2]) == 15.0);
    return 0;
}
