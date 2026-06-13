// ctor2.cpp
//
// Two C++11 features kill constructor boilerplate:
//
//   DEFAULT MEMBER INITIALIZERS (NSDMI) — the default lives AT THE
//   MEMBER, once:
//       int seconds = 30;
//   Every constructor whose init list doesn't mention `seconds` uses it.
//
//   DELEGATING CONSTRUCTORS — a constructor can forward to another:
//       Timer(int s) : Timer(s, false) {}
//   One constructor holds the real logic; the rest are one-liners.
//   (A delegating constructor may not ALSO have a member init list —
//   delegate or initialize, not both.)
//
// Before C++11 the same class needed the defaults copy-pasted into three
// constructors — and they'd drift apart over the years. That drift is
// this exercise: someone added the (int) constructor and got the default
// wrong.
//
// Task: 1) move the default for `seconds` (30) into an NSDMI,
//       2) make Timer(int) delegate to Timer(int, bool),
//       3) Timer() can then be `= default`.

#include <cassert>

struct Timer {
    int seconds;
    bool repeating;

    Timer() {
        seconds = 30;
        repeating = false;
    }

    Timer(int s) {
        seconds = s;
        repeating = true;     // drift! every other ctor defaults this false
    }

    Timer(int s, bool r) {
        seconds = s;
        repeating = r;
    }
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

// I AM NOT DONE
