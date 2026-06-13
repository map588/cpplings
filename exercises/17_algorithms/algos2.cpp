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
// THE ERASE–REMOVE IDIOM completes the job:
//
//     v.erase(std::remove(v.begin(), v.end(), 0), v.end());
//             |---- partition keepers ----------|  ^ chop the tail
//
// C++20 finally shipped the spelling everyone wanted:
//
//     std::erase(v, 0);                  // by value
//     std::erase_if(v, pred);            // by predicate
//     // free functions; they also return how many went away
//
// Task: dropped_packets() is half an idiom. Finish it — once the old
// way (do it, you'll read it in every pre-2020 codebase), then redo it
// with std::erase.

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
