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
//     class Wallet {
//         friend bool can_afford(const Wallet& w, int price) {
//             return w.balance_ >= price;   // full access to privates
//         }
//         int balance_ = 0;
//     };
//
// Such a function is NOT a member (no this; it takes the object as a
// parameter) and is NOT visible to ordinary lookup — it can only be
// found by ADL (module 05!), i.e. by calling can_afford(w, 5) with a
// Wallet argument. Benefits: the global namespace stays clean, the
// overload set stays small, and conversions can't sneak into the
// argument. It's how operator<< for your types is usually written.
//
// Task: give is_freezing and operator<< access to celsius_ — as hidden
// friends.
//   - compiles and every assert passes
//   - both stay NON-members (no this; the object arrives as a
//     parameter) and remain callable exactly as main calls them
// Constraints:
//   - celsius_ stays private — no getters, no making it public
//   - don't change main

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
