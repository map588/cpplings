// move7.cpp — solution

#include <cassert>
#include <utility>

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
    // 1. Guaranteed elision: constructed once, in place. 0 / 0.
    Tracker::reset();
    Tracker a = make();
    assert(Tracker::copies == 0 && Tracker::moves == 0);

    // 2. Copy init: 1 copy.
    Tracker::reset();
    Tracker b = a;
    assert(Tracker::copies == 1 && Tracker::moves == 0);

    // 3. Move init: 1 move.
    Tracker::reset();
    Tracker c = std::move(a);
    assert(Tracker::copies == 0 && Tracker::moves == 1);

    // 4. Temporary elides straight into the parameter: 0 / 0.
    Tracker::reset();
    sink(Tracker{});
    assert(Tracker::copies == 0 && Tracker::moves == 0);

    // 5. Moved lvalue: 1 move into the parameter.
    Tracker::reset();
    sink(std::move(b));
    assert(Tracker::copies == 0 && Tracker::moves == 1);

    // 6. const blocks the move ctor → copy ctor silently wins: 1 copy.
    const Tracker frozen;
    Tracker::reset();
    Tracker d = std::move(frozen);
    assert(Tracker::copies == 1 && Tracker::moves == 0);

    (void)c; (void)d;
    return 0;
}
