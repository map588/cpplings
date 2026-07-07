// algos2.cpp
//
// The most-asked C++ interview question of its era: why didn't
// std::remove remove anything?
//
//     std::remove(v.begin(), v.end(), 0);
//     // v.size() unchanged?!
//
// Because remove CAN'T remove. It receives two iterators — it doesn't
// know the container exists, let alone how to shrink it. What it does:
// shift every keeper toward the front, return an iterator to the new
// LOGICAL end. The tail behind it: moved-from junk, still inside v.
//
// Completing the job is the famous ERASE–REMOVE IDIOM: remove
// partitions the keepers to the front and hands you the new logical
// end; the container's own erase must then chop the tail — only a
// member knows how to shrink. C++20 finally shipped free functions
// (named after what everyone always wished remove did) that do both
// steps in one call AND report how many elements went away.
//
// Task: make drop_zeros() actually drop the zeros AND report the count.
//   - readings ends up {5, 7, 3}; dropped == 3
//   - the vector must genuinely shrink — no moved-from junk left in
//     the tail
// Constraints:
//   - no hand-written loops; no extra pass over the data just to count
//   - don't change main() or the asserts

#include <algorithm>
#include <cassert>
#include <vector>

// Removes all zero readings, returns how many were dropped.
std::size_t drop_zeros(std::vector<int>& readings) {
    std::size_t before = readings.size();
    std::remove(readings.begin(), readings.end(), 0);
    return before - readings.size();
}

int main() {
    std::vector<int> readings = {5, 0, 7, 0, 0, 3};

    std::size_t dropped = drop_zeros(readings);

    assert(dropped == 3);
    assert(readings == (std::vector<int>{5, 7, 3}));
    return 0;
}

// I AM NOT DONE
