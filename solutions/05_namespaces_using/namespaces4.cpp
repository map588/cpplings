// namespaces4.cpp — solution

#include <cassert>
#include <utility>
#include <vector>

namespace geo {

struct Mesh {
    std::vector<int> vertices;
};

inline int custom_swaps = 0;

void swap(Mesh& a, Mesh& b) {
    ++custom_swaps;
    a.vertices.swap(b.vertices);
}

}  // namespace geo

void shuffle(geo::Mesh& a, geo::Mesh& b) {
    // The swap two-step: fallback in scope, call unqualified, let ADL
    // find geo::swap because the arguments are geo::Mesh.
    using std::swap;
    swap(a, b);
}

int main() {
    geo::Mesh m1{{1, 2, 3}};
    geo::Mesh m2{{9}};

    shuffle(m1, m2);

    assert(m1.vertices.size() == 1 && m2.vertices.size() == 3);
    assert(geo::custom_swaps == 1);
    return 0;
}
