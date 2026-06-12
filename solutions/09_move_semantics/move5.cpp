// move5.cpp — solution

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
    // Sink: take by value, move into place.
    void set_tag(Tag t) {
        tag_ = std::move(t);
    }

private:
    Tag tag_;
};

int main() {
    Profile p;

    Tag::copies = 0;
    Tag::moves = 0;
    p.set_tag(Tag{});                 // elided into t, moved into tag_
    assert(Tag::copies == 0);
    assert(Tag::moves == 1);

    Tag keep;
    Tag::copies = 0;
    Tag::moves = 0;
    p.set_tag(keep);                  // copied into t, moved into tag_
    assert(Tag::copies == 1);
    assert(Tag::moves == 1);
    return 0;
}
