// move7.cpp  — QUIZ for module 09 (and a new gotcha to close on)
//
// Everything from move1–move6, plus the silent killer:
//
//   MOVING FROM CONST DOESN'T MOVE. std::move(const_obj) yields
//   `const T&&` — and move constructors take `T&&`, non-const (they
//   must vandalize the source!). Overload resolution shrugs and picks
//   the copy constructor, which binds anything. It compiles. It copies.
//   No warning at any -W level. `const` and "movable" are enemies:
//   a const local you intend to move from shouldn't be const.
//
// Predict every counter. The TODO values are lies.

#include <cassert>
#include <utility>

constexpr int TODO = -1;

struct Tracker {
    inline static int copies = 0;
    inline static int moves = 0;
    Tracker() = default;
    Tracker(const Tracker&) { ++copies; }
    Tracker& operator=(const Tracker&) { ++copies; return *this; }
    Tracker(Tracker&&) noexcept { ++moves; }
    Tracker& operator=(Tracker&&) noexcept { ++moves; return *this; }
    static void reset() { copies = moves = 0; }
};

Tracker make() { return Tracker{}; }

void sink(Tracker t) { (void)t; }

int main() {
    // 1. Guaranteed elision (move4): a factory return costs...
    Tracker::reset();
    Tracker a = make();
    assert(Tracker::copies == TODO && Tracker::moves == TODO);

    // 2. Plain copy init from an lvalue:
    Tracker::reset();
    Tracker b = a;
    assert(Tracker::copies == TODO && Tracker::moves == TODO);

    // 3. Move init from an lvalue:
    Tracker::reset();
    Tracker c = std::move(a);
    assert(Tracker::copies == TODO && Tracker::moves == TODO);

    // 4. Sink fed a temporary (move5):
    Tracker::reset();
    sink(Tracker{});
    assert(Tracker::copies == TODO && Tracker::moves == TODO);

    // 5. Sink fed a moved lvalue:
    Tracker::reset();
    sink(std::move(b));
    assert(Tracker::copies == TODO && Tracker::moves == TODO);

    // 6. The const trap — read the header comment, then predict:
    const Tracker frozen;
    Tracker::reset();
    Tracker d = std::move(frozen);
    assert(Tracker::copies == TODO && Tracker::moves == TODO);

    (void)c; (void)d;
    return 0;
}

// I AM NOT DONE
