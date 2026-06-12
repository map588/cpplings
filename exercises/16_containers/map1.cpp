// map1.cpp
//
// std::map's operator[] is a WRITE operation in a read costume: if the
// key is missing, it INSERTS a default-constructed value and returns a
// reference to it. Perfect for accumulation:
//
//     ++counts[word];          // first sight of `word` starts it at 0
//
// Catastrophic for queries:
//
//     if (scores["alice"] > 0)     // alice wasn't there. NOW SHE IS,
//                                  // with score 0. The map grew on a
//                                  // "read" — and on a const map this
//                                  // doesn't even compile.
//
// Query tools that don't mutate:
//     m.contains(key)          // C++20, the obvious one at last
//     m.find(key)              // iterator, or end() — when you also
//                              // want the value (one lookup, not two)
//     m.count(key)             // the pre-C++20 idiom you'll still read
//
// Also on display below: std::map iterates in SORTED key order — it's a
// red-black tree, not a hash table. (Need raw speed and no order?
// unordered_map. Need order or stable iterators? map.)
//
// Task: has_passed() pollutes the map. Make it actually read-only —
// signature included.

// I AM NOT DONE

#include <cassert>
#include <map>
#include <string>

bool has_passed(std::map<std::string, int>& scores,
                const std::string& name) {
    return scores[name] >= 60;
}

int main() {
    std::map<std::string, int> scores;
    ++scores["zoe"];                     // operator[] for accumulation: yes
    scores["zoe"] += 89;
    scores["abe"] = 72;

    assert(scores.size() == 2);

    assert(has_passed(scores, "zoe"));
    assert(!has_passed(scores, "ghost"));    // a query...
    assert(scores.size() == 2);              // ...should not add players!

    // Sorted iteration is part of map's contract:
    std::string order;
    for (const auto& [name, score] : scores) {
        order += name[0];
    }
    assert(order == "az");
    return 0;
}
