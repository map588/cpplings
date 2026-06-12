// move5.cpp
//
// API design: how should a "store this" function take its parameter?
//
//   void set_name(const std::string& n) { name_ = n; }
//     lvalue arg:  1 copy (the assignment)
//     rvalue arg:  1 copy — the const& happily binds the temporary
//                  (module 03), and then you COPY it. The caller's
//                  std::move accomplished nothing!
//
//   void set_name(std::string n) { name_ = std::move(n); }   // SINK
//     lvalue arg:  1 copy (into n) + 1 cheap move
//     rvalue arg:  0 copies — the temporary is elided straight into n
//                  (move4!), then 1 move into place
//
// The by-value "sink" parameter is one overload that's optimal-ish for
// both cases. The const& version is never better and is strictly worse
// for rvalues. (The third option — overloading on const&/&& — wins a
// move but doubles your API; perfect forwarding (move6) generalizes it.)
//
// Task: Profile::set_tag is the const&-and-copy version. The counters in
// main demand sink behavior. Convert it.

// I AM NOT DONE

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
