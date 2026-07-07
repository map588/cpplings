// weak1.cpp
//
// shared_ptr's failure mode: THE CYCLE. If A holds a shared_ptr to B and
// B holds one back to A, each keeps the other's count above zero — both
// destructors wait politely forever. No crash, no error: just objects
// that never die. (Watch `alive` below.)
//
// The fix is std::weak_ptr: it OBSERVES an object managed by shared_ptrs
// without owning it — no contribution to the count. The cost of not
// owning: the object may be gone when you look, so access is a two-step:
//
//     std::weak_ptr<Node> w = sp;
//     if (std::shared_ptr<Node> s = w.lock()) {   // atomically: owner or null
//         use(*s);                                // safe: s keeps it alive
//     }
//
// Design rule for hierarchies: OWNERS POINT DOWN with shared_ptr (or
// better, unique_ptr); children point UP with weak_ptr (or a raw pointer,
// when the parent provably outlives the child). Sideways/back edges:
// weak.
//
// Task: break the cycle so the nodes can actually die.
//   - all asserts pass, including Node::alive == 0 at the end
//   - parent_name(*leaf) still returns "root" while root is alive — and
//     must safely return "(orphan)" if the parent is already gone
// Constraints:
//   - Node keeps both edges (child and parent); the down-edge keeps owning
//   - don't "fix" it by manually clearing an edge before scope exit
//   - don't change any assert

#include <cassert>
#include <memory>
#include <string>

struct Node {
    inline static int alive = 0;

    std::string name;
    std::shared_ptr<Node> child;
    std::shared_ptr<Node> parent;     // the up-edge that owns. Oops.

    explicit Node(std::string n) : name(std::move(n)) { ++alive; }
    ~Node() { --alive; }
};

std::string parent_name(const Node& n) {
    if (auto p = n.parent) {          // compiles only while the up-edge owns —
                                      // this line needs attention after your fix
        return p->name;
    }
    return "(orphan)";
}

int main() {
    {
        auto root = std::make_shared<Node>("root");
        auto leaf = std::make_shared<Node>("leaf");
        root->child = leaf;
        leaf->parent = root;          // cycle complete: root ⇄ leaf

        assert(Node::alive == 2);
        assert(parent_name(*leaf) == "root");
    }
    // Both shared_ptrs left scope... but root held leaf at count 1 and
    // leaf held root at count 1. Nobody died:
    assert(Node::alive == 0);
    return 0;
}

// I AM NOT DONE
