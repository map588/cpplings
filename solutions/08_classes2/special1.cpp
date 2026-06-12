// special1.cpp — solution

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

struct Plain {
    Tracker t;
};

struct Logged {
    Tracker t;
    ~Logged() { /* breakpoint anchor — keep me! */ }

    // The destructor suppressed implicit moves; bring everything back.
    // (Declaring the moves alone would delete the copies, and any ctor
    // suppresses the default ctor — so default the lot.)
    Logged() = default;
    Logged(const Logged&) = default;
    Logged& operator=(const Logged&) = default;
    Logged(Logged&&) = default;
    Logged& operator=(Logged&&) = default;
};

int main() {
    Tracker::reset();
    Plain a;
    Plain b = std::move(a);
    assert(Tracker::moves == 1);
    assert(Tracker::copies == 0);

    Tracker::reset();
    Logged c;
    Logged d = std::move(c);            // a real move again
    assert(Tracker::moves == 1);
    assert(Tracker::copies == 0);       // shipped version: this was 1!

    (void)b; (void)d;
    return 0;
}
