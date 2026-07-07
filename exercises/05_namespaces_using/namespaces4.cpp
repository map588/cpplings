// namespaces4.cpp
//
// ADL — argument-dependent lookup. When you call f(x) UNQUALIFIED, the
// compiler searches not only the enclosing scopes but also THE NAMESPACES
// OF x's TYPE. That's the rule that makes operators work:
//
//   std::cout << 42;   // finds std::operator<<(ostream&, int) — you never
//                      // wrote `std::` for the <<. ADL did it.
//
// ADL also powers customization points, and there's an idiom to learn:
// THE SWAP TWO-STEP. Generic code should arrange two things at once: the
// standard fallback is in scope, and the call itself is UNQUALIFIED — so
// ADL is free to find a cheaper swap the type ships in its own namespace,
// while everything else still lands on the fallback.
//
// Calling std::swap(a, b) QUALIFIED pins the generic version (move tmp =
// a; a = b; b = tmp) and silently skips a cheaper, smarter swap the type
// provides in its own namespace. It compiles. It works. It's just slower —
// the worst kind of bug.
//
// Task: make shuffle() reach geo's own swap — both asserts pass (in
// particular, custom_swaps must end up exactly 1).
// Constraints:
//   - don't call geo::swap by its qualified name — shuffle() shouldn't
//     have to know which namespace owns the better swap
//   - don't touch namespace geo or the asserts

#include <cassert>
#include <utility>
#include <vector>

namespace geo {

struct Mesh {
    std::vector<int> vertices;
};

inline int custom_swaps = 0;   // inline variable (C++17) — module 06!

// Mesh's own swap: just trades the vector guts, no Mesh moves at all.
void swap(Mesh& a, Mesh& b) {
    ++custom_swaps;
    a.vertices.swap(b.vertices);
}

}  // namespace geo

void shuffle(geo::Mesh& a, geo::Mesh& b) {
    std::swap(a, b);   // qualified: ADL never gets a chance
}

int main() {
    geo::Mesh m1{{1, 2, 3}};
    geo::Mesh m2{{9}};

    shuffle(m1, m2);

    assert(m1.vertices.size() == 1 && m2.vertices.size() == 3);  // swapped fine...
    assert(geo::custom_swaps == 1);  // ...but through the wrong machinery
    return 0;
}

// I AM NOT DONE
