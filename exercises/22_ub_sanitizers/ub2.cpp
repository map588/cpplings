// ub2.cpp
//
// operator[] does not check bounds. Not on arrays, not on vectors —
// by design: a check per access would tax every loop in every program.
// The contract is yours: index ∈ [0, size). Break it and you read (or
// worse, WRITE) whatever lives next door — heap metadata, other
// objects, eventually security advisories.
//
// Two different bugs below, needing two different medicines:
//
//   sum_all's loop overshoots by exactly one element — the most common
//   out-of-bounds in existence. ASan's heap-buffer-overflow report
//   shows the bad read's offset and the allocation it tramples past.
//
//   lookup() forwards an index straight from "the network" into
//   unchecked access. A loop bound you can prove correct; hostile
//   input you can't — and vector offers a second way to index, one
//   that checks and THROWS instead of trampling. Untrusted input is
//   exactly where it belongs.
//
// Task: make both functions safe.
//   - sum_all returns 60 and runs clean under ASan
//   - lookup(data, 1) still returns 20; lookup(data, 9999) throws
//     std::out_of_range (main catches it — `caught` must end true)
// Constraints:
//   - don't change main or any assert
//   - sum_all must still visit every element

#include <cassert>
#include <stdexcept>
#include <vector>

int sum_all(const std::vector<int>& v) {
    int total = 0;
    for (std::size_t i = 0; i <= v.size(); ++i) {
        total += v[i];
    }
    return total;
}

int lookup(const std::vector<int>& v, std::size_t user_index) {
    return v[user_index];          // index straight from "the network"
}

int main() {
    std::vector<int> data = {10, 20, 30};

    assert(sum_all(data) == 60);

    assert(lookup(data, 1) == 20);

    bool caught = false;
    try {
        lookup(data, 9999);        // hostile index: should THROW, not read
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
    return 0;
}

// I AM NOT DONE
