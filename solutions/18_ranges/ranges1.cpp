// ranges1.cpp — solution

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

    // Default comparator, age projection:
    std::ranges::sort(people, {}, &Person::age);

    assert(people[0].name == "bob" && people[3].name == "cy");

    auto oldest = std::ranges::max_element(people, {}, &Person::age);
    assert(oldest->name == "cy");

    long n36 = std::ranges::count(people, 36, &Person::age);
    assert(n36 == 2);
    return 0;
}
