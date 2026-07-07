// ctor2.cpp
//
// Two C++11 features kill constructor boilerplate:
//
//   DEFAULT MEMBER INITIALIZERS (NSDMI) — the default lives AT THE
//   MEMBER, once:
//       struct Widget { int size = 64; };
//   Every constructor whose init list doesn't mention `size` uses it.
//
//   DELEGATING CONSTRUCTORS — a constructor can forward to another:
//       Widget() : Widget(64, false) {}
//   One constructor holds the real logic; the rest are one-liners.
//   (A delegating constructor may not ALSO have a member init list —
//   delegate or initialize, not both.)
//
// Before C++11 the same class needed the defaults copy-pasted into three
// constructors — and they'd drift apart over the years. That drift is
// this exercise: someone added the (int) constructor and got the default
// wrong.
//
// Task: fix the wrong default so the drift can never come back.
//   - every assert passes
//   - the default value for each member is written in exactly ONE place
//   - exactly ONE constructor actually initializes members; the others
//     delegate or are compiler-generated
// Constraints:
//   - all three call forms in main must keep working
//   - just flipping `true` to `false` doesn't count — restructure so a
//     future constructor can't repeat the mistake

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
