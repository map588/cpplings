// ctor2.cpp — solution

#include <cassert>

struct Timer {
    int seconds = 30;        // the default, stated exactly once (NSDMI)
    bool repeating = false;

    Timer() = default;                          // NSDMIs do everything
    Timer(int s) : Timer(s, false) {}           // delegates
    Timer(int s, bool r) : seconds(s), repeating(r) {}
};

int main() {
    Timer plain;
    assert(plain.seconds == 30 && !plain.repeating);

    Timer once(5);
    assert(once.seconds == 5 && !once.repeating);

    Timer loop(10, true);
    assert(loop.seconds == 10 && loop.repeating);
    return 0;
}
