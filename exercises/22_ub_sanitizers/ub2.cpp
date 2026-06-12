// ub2.cpp
//
// operator[] does not check bounds. Not on arrays, not on vectors —
// by design: a check per access would tax every loop in every program.
// The contract is yours: index ∈ [0, size). Break it and you read (or
// worse, WRITE) whatever lives next door — heap metadata, other
// objects, eventually security advisories.
//
// The specimen: a <= where < belonged, the most common OOB in
// existence. v[v.size()] is one past the end. ASan's
// heap-buffer-overflow report shows the bad index's offset and what
// allocation it tramples past.
//
// Defense in depth:
//   - range-for when you don't need the index at all (it can't miss)
//   - .at(i) — bounds-checked, throws std::out_of_range; right where
//     the index comes from untrusted input
//   - debug STL modes / hardened libc++ check [] too — in debug builds
//
// Task: fix the loop bound. Then make the lookup with the USER-PROVIDED
// index use .at() — let hostile input throw instead of trample.

// I AM NOT DONE

#include <cassert>
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
