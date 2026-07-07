// functions2.cpp
//
// Default arguments, two rules people learn the hard way:
//
//   1. Once a parameter has a default, every parameter AFTER it needs one
//      too. (Arguments fill in left-to-right; there'd be no way to "skip".)
//      So defaults go on the END — which means parameter ORDER is API
//      design: required things first.
//
//   2. A default is substituted at the CALL SITE and re-evaluated on
//      EVERY call. `int id = next_id()` runs next_id() each time the
//      caller omits the argument. (This is also why a default can't refer
//      to other parameters, and why member defaults capture no `this`.)
//
// Task: make the program compile and pass every assert.
//   - make_tag breaks rule 1 — repair its interface so the existing call
//     sites (which are correct API usage) compile as written
//   - give TODO the number of times next_id really ran
// Constraints:
//   - do not change any line in main()
//   - keep the default prefix "tag" and keep auto_tag's default a call
//     to next_id()

#include <cassert>
#include <string>

constexpr int TODO = -1;

int ids_handed_out = 0;
int next_id() { return ++ids_handed_out; }

// This declaration breaks rule 1 — the compiler will point at it:
std::string make_tag(std::string prefix = "tag", int id) {
    return prefix + std::to_string(id);
}

std::string auto_tag(int id = next_id()) {   // re-evaluated per call
    return "#" + std::to_string(id);
}

int main() {
    assert(make_tag(7) == "tag7");           // uses the default prefix
    assert(make_tag(8, "user") == "user8");  // overrides it

    assert(auto_tag() == "#1");
    assert(auto_tag() == "#2");
    assert(auto_tag(99) == "#99");           // explicit arg: default not run

    assert(ids_handed_out == TODO);          // how many times did it run?
    return 0;
}

// I AM NOT DONE
