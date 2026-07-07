// lambdas5.cpp
//
// Why lambdas exist: the standard algorithms take CALLABLES, and before
// C++11 you had to write a named functor three screens away from the one
// line that used it. Now the logic sits at the call site:
//
//     std::sort(v.begin(), v.end(),
//               [](const auto& a, const auto& b) { return a < b; });
//
// Notes for the two TODOs below:
//
//   - sort's comparator answers "should a come BEFORE b?" — a strict
//     weak ordering, the same contract as operator< (module 08). Decide
//     what "before" means when the longest word goes first.
//   - count_if's predicate answers "does this element count?" — a bool.
//   - `const auto&` parameters make a GENERIC lambda (C++14): the
//     operator() is a template. One comparator works for any container
//     element type.
//
// Task: replace the two /* TODO */s with lambdas.
//   - all asserts pass: words end up longest-first, and big == 2
// Constraints:
//   - lambdas at the call sites — no named functors, no free functions
//   - don't pre-arrange `words` by hand; don't change any assert

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> words = {"fig", "watermelon", "kiwi", "banana"};

    // Sort LONGEST FIRST:
    std::sort(words.begin(), words.end(), /* TODO */);

    assert(words[0] == "watermelon");
    assert(words[1] == "banana");
    assert(words[2] == "kiwi");
    assert(words[3] == "fig");

    // Count the words longer than 4 characters:
    long big = std::count_if(words.begin(), words.end(), /* TODO */);
    assert(big == 2);
    return 0;
}

// I AM NOT DONE
