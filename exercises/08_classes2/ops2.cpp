// ops2.cpp
//
// Before C++20, a fully comparable type meant writing six operators
// (==, !=, <, <=, >, >=) — or generating them from < and == with macro
// tricks. C++20 collapsed all of it into one line:
//
//     auto operator<=>(const Version&) const = default;
//
// The "spaceship" operator returns an ordering (here std::strong_ordering:
// less/equal/greater), and the compiler REWRITES every comparison through
// it: a < b becomes (a <=> b) < 0, and so on. Defaulting <=> also
// implicitly declares a defaulted operator== — so one line really does
// buy all six.
//
// The defaulted version compares members LEXICOGRAPHICALLY IN DECLARATION
// ORDER — for a Version{major, minor, patch} that's exactly semver order,
// but only because the members are declared in that order. Declaration
// order is part of your comparison semantics now; reorder members and you
// reorder sorting.
//
// Task: Version isn't comparable at all yet. Make it so — with one line.

// I AM NOT DONE

#include <cassert>
#include <compare>   // the orderings live here (C++20)

struct Version {
    int major;
    int minor;
    int patch;
};

int main() {
    Version v1_2_3{1, 2, 3};
    Version v1_10_0{1, 10, 0};
    Version dup{1, 2, 3};

    assert(v1_2_3 < v1_10_0);          // 2 < 10: lexicographic, not textual
    assert(v1_10_0 > v1_2_3);
    assert(v1_2_3 == dup);
    assert(v1_2_3 != v1_10_0);
    assert(v1_2_3 <= dup);
    assert(v1_10_0 >= v1_2_3);
    return 0;
}
