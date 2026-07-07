// valcat4.cpp
//
// A T&-returning function (valcat2) is a LIFETIME PROMISE: "the object
// behind this reference outlives me." The promise is only keepable if
// the object belongs to someone who outlives the call — the caller,
// usually, via a reference parameter.
//
// longest_name() below breaks the promise in the sneakiest way
// possible. It does NOT return a reference to a plain local — clang
// would warn about that one. Look closely at what the returned
// reference actually points INTO, and at when that thing dies. ASan
// will narrate the aftermath.
//
// The contract to internalize: REFERENCE IN, REFERENCE OUT. A function
// may hand out a reference into an object the CALLER handed to it by
// reference — the caller's object outlives the call. It must never
// hand out a reference into anything that dies at its own closing
// brace... and more things die there than just the named locals.
//
// Task: make longest_name keep its lifetime promise.
//   - the program runs clean under ASan/UBSan and every assert passes —
//     including the address assert: the result must alias the CALLER's
//     element, no copy anywhere
//   - the fix is tiny; the diagnosis is the exercise
// Constraints:
//   - do not change main
//   - the return type stays const std::string& — no returning by
//     value, no index or pointer workarounds

#include <cassert>
#include <string>
#include <vector>

// Returns a reference to the longest name. Whose longest name, though —
// and how long does that owner live?
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
