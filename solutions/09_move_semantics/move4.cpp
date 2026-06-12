// move4.cpp — solution

#include <cassert>

struct Widget {
    inline static int constructions = 0;
    inline static int moves = 0;

    int value;

    explicit Widget(int v) : value(v) { ++constructions; }
    Widget(Widget&& o) noexcept : value(o.value) { ++moves; }
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    Widget& operator=(Widget&&) noexcept = default;
};

Widget make_prvalue() {
    return Widget{1};      // prvalue: elision guaranteed since C++17
}

Widget make_named() {
    Widget w{2};
    return w;              // bare name: NRVO (and never worse than a move)
}

int main() {
    Widget::constructions = 0;
    Widget::moves = 0;
    Widget a = make_prvalue();
    assert(a.value == 1);
    assert(Widget::constructions == 1);
    assert(Widget::moves == 0);

    Widget::constructions = 0;
    Widget::moves = 0;
    Widget b = make_named();
    assert(b.value == 2);
    assert(Widget::constructions == 1);
    assert(Widget::moves == 0);
    return 0;
}
