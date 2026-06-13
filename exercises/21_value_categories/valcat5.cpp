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
// The && overload is where `return std::move(member_);` is CORRECT —
// the module-09 exception (move4 banned it for locals; a member of an
// expiring *this is exactly what move was invented for).
//
// The Tracker counts what happens to the payload. Ship: one const
// overload, so even the temporary case copies.
//
// Task: add the `const&` / `&&` overload pair for result(). The
// counters define success.

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
