// ranges4.cpp
//
// Views and range algorithms don't own data (string_view rules, module
// 15). But here the library does something remarkable: it makes the
// classic dangling bug FAIL TO COMPILE.
//
//     auto it = std::ranges::max_element(load_scores());
//     //                                 ^ rvalue: dies at the semicolon
//     *it;                               // would be use-after-free...
//
// A range algorithm handed an RVALUE container it can't safely point
// into doesn't return an iterator at all — it returns
// std::ranges::dangling, a placeholder type with NO operator*. The
// error message literally names the problem.
//
// (How does it know lvalues are fine? "Borrowed ranges": an lvalue
// container outlives the statement, so iterators into it may leave.
// Some types — string_view, span — are SAFE even as rvalues, because
// their iterators point at memory the view never owned anyway. They're
// marked `enable_borrowed_range`, and algorithms on them return real
// iterators.)
//
// Task: name the container, then call the algorithm on the lvalue.

#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

std::vector<int> load_scores() {
    return {88, 42, 95, 61};
}

int main() {
    auto best = std::ranges::max_element(load_scores());
    assert(*best == 95);
    return 0;
}

// I AM NOT DONE
