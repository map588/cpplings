// ring1.cpp
//
// A fixed-capacity ring buffer (no heap allowed).
//
// Rule one of firmware and hard-real-time C++: no dynamic allocation
// after startup. Not superstition — three concrete failure modes:
//   - fragmentation: a heap churned for weeks can refuse a 64-byte
//     request while holding kilobytes free, in useless slivers;
//   - latency: malloc takes however long it takes — a deadline
//     scheduler can't budget "however long";
//   - failure: bad_alloc in an interrupt handler has no sane catcher.
//
// So the workhorse container is a fixed-capacity RING buffer: a
// compile-time-sized std::array (C++11) living inside the object, plus
// two integers. UART receive buffers, log rings, sensor sample windows
// — this exact shape, everywhere. When the ring is full, the newest
// element overwrites the OLDEST (dropping a stale sample beats blocking
// the interrupt handler).
//
// The whole trick is index arithmetic that WRAPS: the slot after
// buf_[N - 1] is buf_[0] again. N below is 5 on purpose — not a power
// of two — so bit-mask shortcuts won't accidentally work; think in
// modular arithmetic. (To make "no heap" a checked claim, not a vibe,
// global operator new is instrumented below — module 23's trick.)
//
// Task: complete push() and pop() so every assert passes and the
// program exits 0 (the watcher builds with ASan/UBSan).
//   - push appends at the logical end; on a FULL ring it overwrites the
//     oldest element and size stays N,
//   - pop removes and returns the OLDEST remaining element (FIFO),
//   - the allocation counter must still read zero at the end.
// Constraints:
//   - storage stays std::array<T, N> — no vector/deque/list/new
//     anywhere (the instrumented counter will catch you),
//   - keep the member meanings: head_ = index of the oldest element,
//     count_ = number of live elements,
//   - don't change N, main(), or any assert.

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <new>

// No-heap, audited: count every global allocation (see 23_memory_layout).
inline int news = 0;

void* operator new(std::size_t n) {
    ++news;
    if (void* p = std::malloc(n)) return p;
    throw std::bad_alloc{};
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

template <class T, std::size_t N>
class Ring {
    static_assert(N > 0, "a zero-slot ring holds nothing");

public:
    std::size_t size()  const { return count_; }
    bool        empty() const { return count_ == 0; }
    bool        full()  const { return count_ == N; }

    // Newest element in at the logical end; if already full, the oldest
    // is overwritten (and is no longer the oldest — what is?).
    void push(const T& v) {
        (void)v;                             // TODO
    }

    // Remove and return the oldest element. Precondition: !empty().
    T pop() {
        assert(!empty());
        return T{};                          // TODO
    }

private:
    std::array<T, N> buf_{};   // the ONLY storage — lives inside the object
    std::size_t head_  = 0;    // index of the OLDEST element
    std::size_t count_ = 0;    // number of live elements
};

int main() {
    news = 0;

    Ring<int, 5> r;
    assert(r.empty() && !r.full());

    // Partial fill: plain FIFO, no wrapping yet.
    r.push(1);
    r.push(2);
    r.push(3);
    assert(r.size() == 3 && !r.full());
    assert(r.pop() == 1);
    assert(r.pop() == 2);
    assert(r.size() == 1);

    // Exact fill: 3 is still inside; four more make exactly 5.
    r.push(4);
    r.push(5);
    r.push(6);
    r.push(7);
    assert(r.full() && r.size() == 5);

    // Overflow by 3: the ring keeps the 5 NEWEST — 3, 4, 5 are gone.
    r.push(8);
    r.push(9);
    r.push(10);
    assert(r.size() == 5);
    assert(r.pop() == 6);
    assert(r.pop() == 7);
    assert(r.pop() == 8);
    assert(r.pop() == 9);
    assert(r.pop() == 10);
    assert(r.empty());

    // Interleaved push/pop across the wrap seam: 8 pushes into 5 slots
    // (never overflowing) — the indexes MUST wrap back through 0.
    Ring<int, 5> s;
    int oldest = 0;
    for (int v = 0; v < 8; v += 2) {
        s.push(v);
        s.push(v + 1);
        assert(s.pop() == oldest);
        ++oldest;
    }
    assert(s.size() == 4);
    assert(s.pop() == 4 && s.pop() == 5 && s.pop() == 6 && s.pop() == 7);

    assert(news == 0);   // the whole run: not one heap allocation
    return 0;
}

// I AM NOT DONE
