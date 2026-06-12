// containers1.cpp — solution

#include <cassert>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

template <typename T>
std::uintptr_t addr(const T& x) {
    return reinterpret_cast<std::uintptr_t>(&x);
}

int main() {
    // 1. Growth reallocates: the element moved. false.
    std::vector<int> v;
    v.push_back(1);
    auto v_before = addr(v[0]);
    for (int i = 0; i < 1000; ++i) v.push_back(i);
    bool vector_element_stayed = (addr(v[0]) == v_before);
    assert(vector_element_stayed == false);

    // 2. map is node-based: the element never moves. true.
    std::map<int, std::string> m;
    m[500] = "anchor";
    auto m_before = addr(m[500]);
    for (int i = 0; i < 1000; ++i) m[i] = "x";
    bool map_element_stayed = (addr(m[500]) == m_before);
    assert(map_element_stayed == true);

    // 3. Rehashing rebuilds the BUCKET ARRAY; the nodes stay put.
    //    References/pointers survive (iterators would not!). true.
    std::unordered_map<int, std::string> u;
    u[42] = "anchor";
    auto u_before = addr(u[42]);
    for (int i = 1000; i < 3000; ++i) u[i] = "x";
    bool unordered_element_stayed = (addr(u[42]) == u_before);
    assert(unordered_element_stayed == true);
    return 0;
}
