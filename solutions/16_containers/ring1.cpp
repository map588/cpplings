// ring1.cpp — solution
// The next free slot is (head_ + count_) % N; overwriting a full ring
// advances head_ too. All storage is the in-object std::array — zero heap.

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

    void push(const T& v) {
        buf_[(head_ + count_) % N] = v;      // logical end, wrapped
        if (full())
            head_ = (head_ + 1) % N;         // overwrote the oldest
        else
            ++count_;
    }

    T pop() {
        assert(!empty());
        T v = buf_[head_];
        head_ = (head_ + 1) % N;
        --count_;
        return v;
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
