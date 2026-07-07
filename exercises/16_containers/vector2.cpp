// vector2.cpp
//
// Erasing while iterating — everyone writes this bug exactly once:
//
//     for (auto it = v.begin(); it != v.end(); ++it)
//         if (bad(*it)) v.erase(it);          // and then ++it. Oops.
//
// erase(it) shifts the tail left; the element AFTER the erased one is
// now at `it`'s position. The loop's ++it steps over it — every element
// right after an erased one escapes inspection. (And `it` is formally
// invalidated; with a reallocation in play this graduates from wrong
// to undefined.)
//
// The standard's escape hatch: erase() RETURNS a fresh, valid iterator
// to the element that slid into the hole. A correct loop therefore
// advances EITHER by incrementing OR by taking what erase hands back —
// never both for the same element.
//
// (C++20 buried this whole pattern under a single library call, which
// module 17 celebrates. Learn the idiom anyway: you'll read it in every
// pre-2020 codebase, and write it for maps.)
//
// Task: fix drop_failing_grades() so no element escapes inspection.
//   - every grade below 60 is removed; survivors keep their order
//   - the assert passes (the CONSECUTIVE failures in the data are
//     there precisely to spring the trap)
// Constraints:
//   - erase in place: no second vector, no copy-and-filter
//   - each element is inspected exactly once
//   - don't change main() or the assert

#include <cassert>
#include <vector>

void drop_failing_grades(std::vector<int>& grades) {
    for (auto it = grades.begin(); it != grades.end(); ++it) {
        if (*it < 60) {
            grades.erase(it);
        }
    }
}

int main() {
    //                       v--v consecutive failures: the trap springs
    std::vector<int> grades = {55, 58, 90, 72, 40, 45, 88};
    drop_failing_grades(grades);

    assert(grades == (std::vector<int>{90, 72, 88}));
    return 0;
}

// I AM NOT DONE
