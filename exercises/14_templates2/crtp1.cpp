// crtp1.cpp
//
// templates2 said it plainly: Stack<int> and Stack<std::string> share
// source code and NOTHING else. Every instantiation of a class template
// is a separate class — with its own static members.
//
// Turn that fact into a tool. A base class that takes THE DERIVED CLASS
// as its template argument gets stamped out once per derived class:
//
//     template <typename Derived>
//     struct Base { ... };
//
//     struct Apple  : Base<Apple>  { ... };   // its own Base<Apple>
//     struct Orange : Base<Orange> { ... };   // its own Base<Orange>
//
// A class inheriting from a template of ITSELF looks circular. It
// isn't — Base<Apple> only needs the NAME Apple to be instantiated —
// and it is common enough to have a name: CRTP, the Curiously
// Recurring Template Pattern.
//
// What it buys, in two flavors:
//
//   - per-type state and code, written once (this exercise): each
//     derived class gets its own copy of whatever the base declares
//   - "static polymorphism": the base knows Derived at compile time,
//     so it can call static_cast<Derived&>(*this).impl() — a call the
//     compiler resolves and inlines, with no vtable and no vptr.
//     (std::enable_shared_from_this and std::ranges::view_interface
//     are CRTP bases. C++23's "deducing this" covers many uses with
//     less ceremony — reading note.)
//
// Below, Counted counts live objects so a leak report can say how many
// Widgets and how many Gadgets are alive. As written there is ONE
// Counted class and therefore ONE counter: Widget::alive and
// Gadget::alive are the same variable.
//
// Task: give every derived type its own counter, with Counted written
// once.
//   - every assert passes: Widgets and Gadgets are counted separately
//   - the static_assert holds: the counter machinery adds no bytes to
//     a Widget (no vptr — nothing here is virtual)
// Constraints:
//   - one Counted, used by both types: no second copy of the counting
//     logic, no counter members written into Widget or Gadget
//   - nothing virtual
//   - don't change main or the asserts

#include <cassert>

struct Counted {
    inline static int alive = 0;          // one class → one counter

    Counted() { ++alive; }
    Counted(const Counted&) { ++alive; }  // a copy is a new object too
    ~Counted() { --alive; }
};

struct Widget : Counted {
    int id = 0;
};

struct Gadget : Counted {
    double mass = 0.0;
};

int main() {
    Widget w1, w2;
    Gadget g;
    assert(Widget::alive == 2);           // two Widgets...
    assert(Gadget::alive == 1);           // ...and one Gadget

    {
        Widget copy = w1;
        assert(Widget::alive == 3);
        assert(Gadget::alive == 1);       // a new Widget is not a Gadget
        (void)copy;
    }
    assert(Widget::alive == 2);

    static_assert(sizeof(Widget) == sizeof(int),
                  "the counting base must cost no bytes per object");
    (void)w2; (void)g;
    return 0;
}

// I AM NOT DONE
