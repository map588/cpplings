// strings1.cpp
//
// Where do a string's characters actually LIVE? For short strings:
// inside the std::string object itself — the Small String Optimization.
// The object is ~24 bytes; rather than waste them on a pointer to a
// 3-byte allocation, the implementation stores the characters in place.
// Only strings that don't fit get a heap buffer.
//
// You can catch it red-handed by checking whether .data() points INSIDE
// the object:
//
//     bool inside = sv.data() >= lo && sv.data() < hi
//     // where [lo, hi) is the object's own memory
//
// Consequences worth knowing:
//   - copying a short string is a memcpy of the object — genuinely cheap
//   - MOVING a short string also copies (module 09's "theft" needs a
//     heap buffer to steal — that's why move1 used a long string!)
//   - capacity() starts at the SSO size, not 0
//
// Task: predict the three TODOs. (Exact SSO capacity varies — libc++ 22,
// libstdc++ 15 — the strings below are chosen to behave the same on
// both.)

// I AM NOT DONE

#include <cassert>
#include <string>

constexpr bool TODO_BOOL = false;   // replace each use

bool chars_inside_object(const std::string& s) {
    const char* lo = reinterpret_cast<const char*>(&s);
    const char* hi = lo + sizeof(std::string);
    return s.data() >= lo && s.data() < hi;
}

int main() {
    std::string tiny = "cat";                       // 3 chars
    std::string huge(100, 'x');                     // 100 chars

    assert(chars_inside_object(tiny) == TODO_BOOL);
    assert(chars_inside_object(huge) == TODO_BOOL);

    // An empty string never allocates — its capacity is the SSO buffer:
    std::string empty;
    assert((empty.capacity() > 0) == TODO_BOOL);

    // And this is why moving a short string leaves the source intact-ish
    // while moving a long one visibly steals (compare move1!):
    std::string tiny2 = std::move(tiny);
    std::string huge2 = std::move(huge);
    assert(tiny2 == "cat" && huge2.size() == 100);
    return 0;
}
