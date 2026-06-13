// containers1.cpp  — QUIZ for module 16: the invalidation rules
//
// Per container, which operations move your data? The cheat sheet:
//
//   vector   growth: EVERYTHING dies (pointers, refs, iterators).
//            erase: everything from the erased point onward.
//            reserve()'d headroom: push_back invalidates only end().
//   map/set  node-based: elements NEVER move. Inserts invalidate
//            nothing; erase kills only the erased element's handles.
//   unordered_map
//            rehash (load factor exceeded): all ITERATORS die, but
//            POINTERS AND REFERENCES to elements SURVIVE — the nodes
//            stay put, only the bucket array is rebuilt. The subtlest
//            rule in the table.
//
// Each demo below captures an address before a storm of inserts, then
// checks whether it still matches after. (We compare saved NUMERIC
// addresses — uintptr_t — never dereference a possibly-dead pointer.)
//
// Task: predict the three TODOs.

#include <cassert>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

constexpr bool TODO_BOOL = false;   // replace each use

template <typename T>
std::uintptr_t addr(const T& x) {
    return reinterpret_cast<std::uintptr_t>(&x);
}

int main() {
    // 1. vector, no reserve, forced growth:
    std::vector<int> v;
    v.push_back(1);
    auto v_before = addr(v[0]);
    for (int i = 0; i < 1000; ++i) v.push_back(i);
    bool vector_element_stayed = (addr(v[0]) == v_before);
    assert(vector_element_stayed == TODO_BOOL);

    // 2. map under heavy insertion:
    std::map<int, std::string> m;
    m[500] = "anchor";
    auto m_before = addr(m[500]);
    for (int i = 0; i < 1000; ++i) m[i] = "x";
    bool map_element_stayed = (addr(m[500]) == m_before);
    assert(map_element_stayed == TODO_BOOL);

    // 3. unordered_map through (several!) rehashes:
    std::unordered_map<int, std::string> u;
    u[42] = "anchor";
    auto u_before = addr(u[42]);
    for (int i = 1000; i < 3000; ++i) u[i] = "x";   // rehashes guaranteed
    bool unordered_element_stayed = (addr(u[42]) == u_before);
    assert(unordered_element_stayed == TODO_BOOL);  // nodes vs buckets!
    return 0;
}

// I AM NOT DONE
