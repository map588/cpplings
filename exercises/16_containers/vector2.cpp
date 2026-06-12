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
// The idiom: erase() RETURNS the iterator to the shifted-in element.
// Branch on it:
//
//     for (auto it = v.begin(); it != v.end(); /* nothing! */) {
//         if (bad(*it)) it = v.erase(it);     // erase advances us
//         else ++it;                          // we advance ourselves
//     }
//
// (C++20 buried this whole exercise under one line — std::erase_if(v,
// pred) — which module 17 celebrates. Learn the idiom anyway: you'll
// read it in every codebase, and write it for maps.)
//
// Task: drop_failing_grades skips survivors standing behind the fallen.
// Apply the idiom.

// I AM NOT DONE

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
