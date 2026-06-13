// friend1.cpp
//
// `friend` grants a specific function (or class) access to your privates.
// It's not a failure of encapsulation — it's PART of it: the friend list
// is in the class definition, so the set of code that can touch the
// internals is still written in exactly one place.
//
// The pattern worth learning is the HIDDEN FRIEND idiom: declare AND
// DEFINE the friend inside the class:
//
//     class Temperature {
//         friend bool is_freezing(const Temperature& t) {
//             return t.celsius_ <= 0.0;
//         }
//     };
//
// Such a function is NOT a member (no this; it takes the object as a
// parameter) and is NOT visible to ordinary lookup — it can only be
// found by ADL (module 05!), i.e. by calling is_freezing(temp) with a
// Temperature argument. Benefits: the global namespace stays clean, the
// overload set stays small, and conversions can't sneak into the
// argument. It's how operator<< for your types is usually written.
//
// Task: is_freezing and operator<< both need access to celsius_.
// Move them into the class as hidden friends.

#include <cassert>
#include <sstream>

class Temperature {
public:
    explicit Temperature(double celsius) : celsius_(celsius) {}

private:
    double celsius_;
};

bool is_freezing(const Temperature& t) {
    return t.celsius_ <= 0.0;              // private!
}

std::ostream& operator<<(std::ostream& os, const Temperature& t) {
    return os << t.celsius_ << "°C";       // private!
}

int main() {
    Temperature mild(21.5);
    Temperature cold(-4.0);

    assert(!is_freezing(mild));            // found via ADL
    assert(is_freezing(cold));

    std::ostringstream out;
    out << mild;                           // ADL again — same mechanism
    assert(out.str() == "21.5°C");
    return 0;
}

// I AM NOT DONE
