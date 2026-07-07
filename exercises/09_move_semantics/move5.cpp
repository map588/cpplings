// move5.cpp
//
// API design: how should a "store this" function take its parameter?
//
//   Option A — take a reference-to-const and assign from it:
//     lvalue arg:  1 copy (the assignment)
//     rvalue arg:  ALSO 1 copy — the const& happily binds the temporary
//                  (module 03), and then you COPY it. The caller's
//                  std::move accomplished nothing!
//
//   Option B — the SINK: accept the parameter BY VALUE, then hand that
//   value onward into the member:
//     lvalue arg:  1 copy (into the parameter) + 1 cheap move
//     rvalue arg:  0 copies — the temporary is elided straight into the
//                  parameter (move4!), then 1 move into place
//
// The by-value "sink" parameter is one overload that's optimal-ish for
// both cases. Option A is never better and is strictly worse for
// rvalues. (The third option — overloading on const&/&& — wins a move
// but doubles your API; perfect forwarding (move6) generalizes it.)
//
// Task: convert Profile::set_tag into a sink.
//   - every assert passes — a temporary costs {0 copies, 1 move}, a
//     kept lvalue costs {1 copy, 1 move}
// Constraints:
//   - exactly ONE set_tag (no const&/&& overload pair)
//   - don't change Tag, main, or the asserts

#include <cassert>
#include <utility>

struct Tag {
    inline static int copies = 0;
    inline static int moves = 0;

    Tag() = default;
    Tag(const Tag&) { ++copies; }
    Tag& operator=(const Tag&) { ++copies; return *this; }
    Tag(Tag&&) noexcept { ++moves; }
    Tag& operator=(Tag&&) noexcept { ++moves; return *this; }
};

class Profile {
public:
    void set_tag(const Tag& t) {
        tag_ = t;
    }

private:
    Tag tag_;
};

int main() {
    Profile p;

    // Caller hands over a TEMPORARY — this should cost zero copies:
    Tag::copies = 0;
    Tag::moves = 0;
    p.set_tag(Tag{});                 // temporary elides into the param...
    assert(Tag::copies == 0);         // ...so no copy anywhere
    assert(Tag::moves == 1);          // one move into the member

    // Caller keeps their lvalue — one copy is the honest price:
    Tag keep;
    Tag::copies = 0;
    Tag::moves = 0;
    p.set_tag(keep);
    assert(Tag::copies == 1);
    assert(Tag::moves == 1);
    return 0;
}

// I AM NOT DONE
