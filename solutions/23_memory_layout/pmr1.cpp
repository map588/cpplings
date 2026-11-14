// pmr1.cpp — solution

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <memory_resource>
#include <new>
#include <vector>

inline int news = 0;

void* operator new(std::size_t n) {
    ++news;
    if (void* p = std::malloc(n)) return p;
    throw std::bad_alloc{};
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

int squares_sum(int n) {
    // The arena lives on this function's stack. The pool bumps a pointer
    // through it; null_memory_resource() is the upstream that refuses, so
    // running out throws std::bad_alloc instead of reaching the heap.
    std::array<std::byte, 1024> arena;
    std::pmr::monotonic_buffer_resource pool(
        arena.data(), arena.size(), std::pmr::null_memory_resource());

    std::pmr::vector<int> scratch(&pool);     // same vector, other memory
    for (int i = 1; i <= n; ++i) {
        scratch.push_back(i * i);
    }
    int total = 0;
    for (int x : scratch) {
        total += x;
    }
    return total;
}   // scratch dies, then pool: the whole arena is released at once

int main() {
    news = 0;
    assert(squares_sum(10) == 385);
    assert(squares_sum(3) == 14);
    assert(news == 0);
    return 0;
}
