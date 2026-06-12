// shared1.cpp — solution

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

std::vector<std::shared_ptr<int>> registry;

void transfer(std::shared_ptr<int> p) {
    registry.push_back(std::move(p));
}

int main() {
    auto a = std::make_shared<int>(42);
    assert(a.use_count() == 1);

    {
        std::shared_ptr<int> b = a;              // copy: 2 owners
        assert(a.use_count() == 2);

        std::shared_ptr<int> c = std::move(b);   // move: still 2 owners
        assert(a.use_count() == 2);
        assert(b == nullptr);
    }                                            // c dies

    assert(a.use_count() == 1);

    transfer(std::move(a));                      // give, don't share
    assert(a == nullptr);
    assert(registry[0].use_count() == 1);

    static_assert(sizeof(std::shared_ptr<int>) == 2 * sizeof(void*));
    static_assert(sizeof(std::unique_ptr<int>) == 1 * sizeof(void*));
    return 0;
}
