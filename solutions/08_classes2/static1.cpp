// static1.cpp — solution

#include <cassert>

class Widget {
public:
    Widget()  { ++live_count; }
    ~Widget() { --live_count; }

    inline static int live_count = 0;   // C++17: declares AND defines
};

int main() {
    assert(Widget::live_count == 0);
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
