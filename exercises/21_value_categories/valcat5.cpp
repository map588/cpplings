// valcat5.cpp
//
// You can overload a member function on the value category OF THE
// OBJECT ITSELF — ref-qualifiers (C++11), the least-known corner of
// this module and the payoff of the whole taxonomy:
//
//     std::string take() const&;   // *this is an lvalue → must copy
//     std::string take() &&;       // *this is EXPIRING → may donate!
//
// Where it matters: a builder/parser that hands out its result.
//
//     Document d = parser.result();              // parser lives on: copy
//     Document d = make_parser(text).result();   // temporary parser:
//                                                // why copy a corpse's
//                                                // organs? MOVE them.
//
// The && overload is also where the module-09 rule gets its one
// exception: move4 banned move-on-return for locals, but a member of
// an EXPIRING *this is exactly what move was invented for — the object
// is a corpse, plundering it is the point.
//
// The Tracker counts what happens to the payload. Ship: one const
// overload, so even the temporary case copies.
//
// Task: make result() category-aware.
//   - both counter asserts pass: a persistent parser COPIES its result
//     out (1 copy, 0 moves); a temporary parser DONATES it (0 copies,
//     1 move)
// Constraints:
//   - do not change Tracker, make_parser, or main
//   - result_ stays a private member; result() still returns Tracker
//     by value in both cases
//   - don't push the choice onto callers — the OBJECT's value category
//     must select the behavior automatically

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

class Parser {
public:
    Tracker result() const {
        return result_;                  // always a copy
    }

private:
    Tracker result_;
};

Parser make_parser() { return Parser{}; }

int main() {
    // Case 1: the parser persists — copying its result is correct.
    Parser keeper;
    Tracker::reset();
    Tracker r1 = keeper.result();
    assert(Tracker::copies == 1 && Tracker::moves == 0);

    // Case 2: the parser is a temporary — its result should be DONATED.
    Tracker::reset();
    Tracker r2 = make_parser().result();
    assert(Tracker::copies == 0 && Tracker::moves == 1);

    (void)r1; (void)r2;
    return 0;
}

// I AM NOT DONE
