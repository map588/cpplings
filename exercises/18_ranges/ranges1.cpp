// ranges1.cpp
//
// The C++20 range algorithms take the CONTAINER (any range, in fact):
//
//     std::ranges::sort(v);                    // begin/end: gone
//
// And they take a PROJECTION — "look at this part of each element".
// Where the classic algorithms made you write comparator-lambda
// boilerplate (lambdas5: `[](a, b) { return a.age < b.age; }`, with the
// ever-present risk of writing the comparison backwards), the ranges
// versions let you hand over just the PIECE of the element to operate
// on — a pointer-to-member is enough. The same projection idea runs
// through the whole algorithm family: sorting, min/max, find, count.
//
// Two things to work out from the signatures (cppreference is fair
// game): where the projection argument slots in, and how to say "just
// use the default comparator" when projecting is all you want.
//
// Task: modernize the three numbered lines with range algorithms.
//   - the program compiles and every assert passes
//   - all three use projections; no lambdas anywhere in the file
// Constraints:
//   - do not give Person an operator< (or any comparison operator)
//   - do not change any assert line

#include <algorithm>
#include <cassert>
#include <ranges>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;
};

int main() {
    std::vector<Person> people = {
        {"ada", 36}, {"bob", 19}, {"cy", 72}, {"dee", 36},
    };

    // 1. Sort by AGE (currently sorts by nothing — it doesn't compile,
    //    Person has no operator<):
    std::ranges::sort(people);

    assert(people[0].name == "bob" && people[3].name == "cy");

    // 2. The oldest person (max_element BY AGE):
    auto oldest = people.begin();   // TODO
    assert(oldest->name == "cy");

    // 3. How many people are exactly 36 (count BY AGE)?
    long n36 = 0;                   // TODO
    assert(n36 == 2);
    return 0;
}

// I AM NOT DONE
