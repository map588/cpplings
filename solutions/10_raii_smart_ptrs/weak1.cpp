// weak1.cpp — solution

#include <cassert>
#include <memory>
#include <string>

struct Node {
    inline static int alive = 0;

    std::string name;
    std::shared_ptr<Node> child;      // owner points down
    std::weak_ptr<Node> parent;       // child observes up — no ownership

    explicit Node(std::string n) : name(std::move(n)) { ++alive; }
    ~Node() { --alive; }
};

std::string parent_name(const Node& n) {
    if (auto p = n.parent.lock()) {   // shared_ptr for the duration, or null
        return p->name;
    }
    return "(orphan)";
}

int main() {
    {
        auto root = std::make_shared<Node>("root");
        auto leaf = std::make_shared<Node>("leaf");
        root->child = leaf;
        leaf->parent = root;          // weak: count untouched

        assert(Node::alive == 2);
        assert(parent_name(*leaf) == "root");
    }
    // root's count hit zero (the weak_ptr didn't hold it) → root died →
    // its child shared_ptr released leaf → leaf died.
    assert(Node::alive == 0);
    return 0;
}
