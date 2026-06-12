// variant2.cpp
//
// Module 11 modeled shapes with virtual dispatch: a base class, a
// vtable per object, heap allocation, and "forgot to override" bugs.
// For a CLOSED set of types, variant + std::visit is the alternative:
//
//     std::variant<Circle, Rect> shape = ...;
//     double a = std::visit(visitor, shape);    // dispatch on active type
//
// The visitor needs an operator() per alternative — and if you MISS
// ONE, it doesn't compile. Exhaustiveness, checked statically. (A new
// virtual override forgotten? Runs the base version, silently —
// abstract1 fought that bug at runtime.)
//
// The classic visitor spelling is the OVERLOADED IDIOM — a struct that
// inherits a pack of lambdas and pulls in all their operator()s
// (modules 13 + 14 colliding productively):
//
//     template <class... Ts>
//     struct overloaded : Ts... { using Ts::operator()...; };
//
//     std::visit(overloaded{
//         [](const Circle& c) { ... },
//         [](const Rect& r)  { ... },
//     }, shape);
//
// Trade-offs vs virtual: no heap, no vptr, values in contiguous
// vectors; but the type set is fixed at compile time and every visitor
// names it. Plugins → virtual. Known set → variant.
//
// Task: area()'s visitor only handles Circle. The compiler is
// withholding approval until Rect and Triangle are covered.

// I AM NOT DONE

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
        // TODO: Rect: w * h
        // TODO: Triangle: base * height / 2
    }, s);
}

int main() {
    std::vector<Shape> shapes = {Circle{1.0}, Rect{3, 4}, Triangle{6, 5}};

    assert(area(shapes[0]) > 3.14 && area(shapes[0]) < 3.15);
    assert(area(shapes[1]) == 12.0);
    assert(area(shapes[2]) == 15.0);
    return 0;
}
