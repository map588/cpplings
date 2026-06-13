// valcat4.cpp
//
// A T&-returning function (valcat2) is a LIFETIME PROMISE: "the object
// behind this reference outlives me." The promise is only keepable if
// the object belongs to someone who outlives the call — the caller,
// usually, via a reference parameter.
//
// longest_name() breaks the promise in the sneakiest way: it takes its
// vector BY VALUE. The parameter is a fresh copy, the returned
// reference points into that copy's heap buffer, and the copy dies at
// the closing brace. The caller holds a reference into freed memory —
// ASan will narrate.
//
// (Returning a reference to a plain local is the same bug; clang even
// warns on that one. The by-value-parameter variant evades the warning
// — the compiler can't know the caller didn't want a reference into
// the argument... which is exactly what makes it a great trap.)
//
// The contract to internalize:  REFERENCE IN, REFERENCE OUT —
//   const T& f(const C& c) { return c.part; }   // caller's object: fine
//   const T& f(C c)        { return c.part; }   // your corpse: never
//
// Task: one character... well, two ampersands of fix. Make the
// parameter a const reference so the returned reference points into
// the CALLER's vector.

#include <cassert>
#include <string>
#include <vector>

// Returns a reference to the longest name... in a doomed local copy.
const std::string& longest_name(std::vector<std::string> names) {
    std::size_t best = 0;
    for (std::size_t i = 1; i < names.size(); ++i) {
        if (names[i].size() > names[best].size()) best = i;
    }
    return names[best];
}

int main() {
    std::vector<std::string> crew = {
        "bo",
        "alexandros-the-extraordinarily-long-named",
        "kit",
    };

    const std::string& winner = longest_name(crew);
    assert(winner == "alexandros-the-extraordinarily-long-named");
    assert(&winner == &crew[1]);     // the reference aliases the CALLER's
    return 0;                        // element — no copy anywhere
}

// I AM NOT DONE
