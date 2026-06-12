// valcat5.cpp — solution

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
    Tracker result() const& {            // lvalue parser: copy out
        return result_;
    }
    Tracker result() && {                 // expiring parser: donate
        return std::move(result_);        // moving a member of an
    }                                     // expiring *this — the
                                          // legitimate return-move
private:
    Tracker result_;
};

Parser make_parser() { return Parser{}; }

int main() {
    Parser keeper;
    Tracker::reset();
    Tracker r1 = keeper.result();
    assert(Tracker::copies == 1 && Tracker::moves == 0);

    Tracker::reset();
    Tracker r2 = make_parser().result();
    assert(Tracker::copies == 0 && Tracker::moves == 1);

    (void)r1; (void)r2;
    return 0;
}
