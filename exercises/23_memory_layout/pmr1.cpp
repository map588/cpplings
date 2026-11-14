// pmr1.cpp
//
// layout5 counted allocations. This exercise makes the count ZERO —
// while still using std::vector.
//
// Every standard container takes an ALLOCATOR: the policy object it
// asks for memory. You have been using the default one all along
// (std::allocator, which calls operator new). C++17 added a second,
// far more practical family in <memory_resource>:
//
//   std::pmr::memory_resource      an interface: "give me n bytes",
//                                  "take them back"
//   std::pmr::vector<T>            a std::vector whose allocator asks
//                                  a memory_resource — passed to its
//                                  CONSTRUCTOR, at runtime (pmr =
//                                  polymorphic memory resource)
//
// and ready-made resources. The one firmware and game loops live on:
//
//   std::pmr::monotonic_buffer_resource pool(buffer, size, upstream);
//
// a BUMP allocator over a buffer YOU provide — a local array, say.
// "Allocate" is: hand out the next bytes and move a pointer. "Free" is:
// nothing. Everything is released at once when the resource dies. No
// heap, no fragmentation, no lock — and a fixed, known cost: ring1's
// three reasons for banning the heap, answered without giving up the
// container.
//
// Mind the third argument. When the buffer runs out, the resource asks
// its UPSTREAM resource for more — and the default upstream is the
// heap: your "no allocation" code would quietly allocate again.
// std::pmr::null_memory_resource() is the upstream that refuses
// (it throws std::bad_alloc): overflow becomes loud instead of silent.
//
// squares_sum() below builds a scratch vector on every call — heap
// traffic in what is supposed to be a hot path. The audit from layout5
// is watching.
//
// Task: make squares_sum() allocation-free.
//   - both sums stay correct and `news` is still 0 at the end
//   - the scratch storage is a buffer on squares_sum's own stack
// Constraints:
//   - keep the vector and its push_back loop — only WHERE its memory
//     comes from changes
//   - running out of buffer must fail loudly: no silent fall back to
//     the heap
//   - don't touch the operator new/delete machinery, main, or the
//     asserts

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <memory_resource>
#include <new>
#include <vector>

// The audit from layout5: count every trip to the global heap.
inline int news = 0;

void* operator new(std::size_t n) {
    ++news;
    if (void* p = std::malloc(n)) return p;
    throw std::bad_alloc{};
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

// 1*1 + 2*2 + ... + n*n, by way of a scratch vector.
int squares_sum(int n) {
    std::vector<int> scratch;                 // the heap, on every call
    for (int i = 1; i <= n; ++i) {
        scratch.push_back(i * i);
    }
    int total = 0;
    for (int x : scratch) {
        total += x;
    }
    return total;
}

int main() {
    news = 0;
    assert(squares_sum(10) == 385);
    assert(squares_sum(3) == 14);
    assert(news == 0);          // not one trip to the heap
    return 0;
}

// I AM NOT DONE
