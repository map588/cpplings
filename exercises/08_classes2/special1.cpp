// special1.cpp
//
// The six special members come with SUPPRESSION RULES — declaring some
// stops the compiler from generating others:
//
//   you declare...            compiler then...
//   any constructor        →  no implicit default ctor   (module 07)
//   destructor OR any copy →  NO IMPLICIT MOVES — std::move quietly
//                             falls back to the copy constructor!
//   any move operation     →  copies are DELETED (use = default to keep)
//
// The middle row is the expensive one. An innocent empty `~Logged() {}`
// (someone wanted a breakpoint there once) means every std::move of a
// Logged COPIES. No warning. Vector reallocations, std::sort, returns —
// all silently copying. This is also why the rule-of-five says five:
// writing the destructor without the moves doesn't break, it just crawls.
//
// The Tracker member counts what actually happens to it.
//
// Task: fill in every TODO with the true count, and make Logged movable
// again.
//   - every assert passes
//   - `Logged d = std::move(c)` must actually MOVE the Tracker — the
//     counters prove it — with ~Logged still in place
// Constraints:
//   - keep the destructor; it's load-bearing, says the comment
//   - don't change Tracker, Plain, or main (other than the TODOs)
//   - re-read the table before declaring anything in Logged: every row
//     you trigger has consequences for the other special members

#include <cassert>
#include <utility>

constexpr int TODO = -1;

struct Tracker {
    inline static int copies = 0;   // inline static: module 06's rule
    inline static int moves = 0;
    Tracker() = default;
    Tracker(const Tracker&) { ++copies; }
    Tracker& operator=(const Tracker&) { ++copies; return *this; }
    Tracker(Tracker&&) noexcept { ++moves; }
    Tracker& operator=(Tracker&&) noexcept { ++moves; return *this; }
    static void reset() { copies = moves = 0; }
};

struct Plain {                  // rule of zero: all six generated
    Tracker t;
};

struct Logged {                 // someone added a destructor...
    Tracker t;
    ~Logged() { /* breakpoint anchor — keep me! */ }
};

int main() {
    Tracker::reset();
    Plain a;
    Plain b = std::move(a);             // generated move ctor → moves t
    assert(Tracker::moves == TODO);
    assert(Tracker::copies == TODO);

    Tracker::reset();
    Logged c;
    Logged d = std::move(c);            // "move"... is it?
    assert(Tracker::moves == TODO);
    assert(Tracker::copies == TODO);

    (void)b; (void)d;
    return 0;
}

// I AM NOT DONE
