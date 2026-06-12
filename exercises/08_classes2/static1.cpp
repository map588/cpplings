// static1.cpp
//
// A `static` data member belongs to the CLASS — one object total, shared
// by every instance, alive for the whole program. (Yes, this is the
// fourth meaning of `static`: TU-local (06), static local (04), static
// member function — no `this` — and this one.)
//
// The historical trap: inside the class,
//
//     static int live_count;        // this is only a DECLARATION
//
// declares but does not DEFINE — no storage is allocated. Use it and the
// LINKER complains: undefined symbol Widget::live_count. (Module 06
// energy — recognize the error text!) The classical fix was an
// out-of-class definition in exactly one .cpp file:
//
//     int Widget::live_count = 0;
//
// C++17's inline variables made that ceremony obsolete:
//
//     inline static int live_count = 0;     // declares AND defines,
//                                           // even in a header
//
// (Tracker in special1 already used this — now you know why it worked.)
//
// Task: fix the declaration the modern way.

// I AM NOT DONE

#include <cassert>

class Widget {
public:
    Widget()  { ++live_count; }
    ~Widget() { --live_count; }

    static int live_count;      // declaration only — no storage!
};

int main() {
    assert(Widget::live_count == 0);    // class-qualified: no object needed
    {
        Widget a;
        Widget b;
        assert(Widget::live_count == 2);
        {
            Widget c;
            assert(Widget::live_count == 3);
        }
        assert(Widget::live_count == 2);
    }
    assert(Widget::live_count == 0);
    return 0;
}
