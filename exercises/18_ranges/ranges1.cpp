// ranges1.cpp
//
// The C++20 range algorithms take the CONTAINER (any range, in fact):
//
//     std::ranges::sort(v);                    // begin/end: gone
//
// And they take a PROJECTION — "look at this part of each element":
//
//     std::ranges::sort(people, {}, &Person::age);
//     //                        ^    ^ project each Person to .age
//     //                        ^ default comparator (std::less)
//
// That replaces the comparator-lambda boilerplate you wrote in
// lambdas5: no `[](a, b) { return a.age < b.age; }`, no risk of writing
// the comparison backwards, and the same projection idea works across
// min/max/find/count:
//
//     std::ranges::max_element(people, {}, &Person::age)
//     std::ranges::count(people, 30, &Person::age)
//
// Task: modernize all three TODO lines with ranges + projections. No
// lambdas needed anywhere.

// I AM NOT DONE

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
