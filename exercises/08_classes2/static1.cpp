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
// out-of-class definition in exactly one .cpp file — pure ceremony.
// C++17 made the ceremony obsolete: the same mechanism that lets a
// module-06 header variable be defined in one place lets a static
// member declare AND define itself on its one line inside the class.
//
// (Tracker in special1 already used it — now you know why that worked.)
//
// Task: give live_count storage, the modern way.
//   - compiles, LINKS, and every assert passes
//   - the fix is confined to the one declaration line inside the class
// Constraints:
//   - no out-of-class definition (that's the pre-C++17 way)
//   - live_count still starts at 0; don't change main or the asserts

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

// I AM NOT DONE
