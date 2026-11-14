// crtp1.cpp — solution

#include <cassert>

// CRTP: one instantiation per derived class, so one `alive` per type.
// Derived is never used inside — being a different template argument
// is the whole job.
template <typename Derived>
struct Counted {
    inline static int alive = 0;

    Counted() { ++alive; }
    Counted(const Counted&) { ++alive; }
    ~Counted() { --alive; }
};

struct Widget : Counted<Widget> {
    int id = 0;
};

struct Gadget : Counted<Gadget> {
    double mass = 0.0;
};

int main() {
    Widget w1, w2;
    Gadget g;
    assert(Widget::alive == 2);           // Counted<Widget>::alive
    assert(Gadget::alive == 1);           // Counted<Gadget>::alive

    {
        Widget copy = w1;
        assert(Widget::alive == 3);
        assert(Gadget::alive == 1);
        (void)copy;
    }
    assert(Widget::alive == 2);

    static_assert(sizeof(Widget) == sizeof(int),
                  "the counting base must cost no bytes per object");
    (void)w2; (void)g;
    return 0;
}
