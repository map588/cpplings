// swap1.cpp
//
// Count the assignment operators you have written for IntBuffer so far:
// copy assignment in copy1 (self-check, free, allocate, copy — four
// steps in an order you must not get wrong), then move assignment in
// move2 (self-check, free, steal, null). Two functions, two fragile
// orders, the same job: "become that other buffer, and let go of mine."
//
// THE COPY-AND-SWAP IDIOM collapses both into one function with no
// order to get wrong. Three moves:
//
//   1. Take the right-hand side BY VALUE. By the time your function
//      body runs, the parameter is a brand-new IntBuffer, built by a
//      constructor you already trust — the copy constructor if the
//      caller passed an lvalue, the move constructor if they passed an
//      rvalue (move5's sink, again). One operator now serves both.
//   2. SWAP guts with the parameter: pointer for pointer, size for
//      size. Trading two pointers cannot fail.
//   3. Return. The parameter dies at the closing brace — holding what
//      used to be YOUR buffer. Its destructor does the freeing.
//
// Look at what fell out for free. Self-assignment: the copy was made
// before anything was freed, so `b = b` is merely wasteful, never
// wrong. Failure: if the copy cannot be made, your object has not been
// touched yet — commit-or-nothing (module 20 names this the strong
// guarantee). Leaks: there is no "free my old buffer" line to forget.
//
// The swap itself belongs to the class as a HIDDEN FRIEND (friend1) —
// a noexcept free function found by ADL, the customization point that
// `using std::swap; swap(a, b);` reaches (namespaces4).
//
// The class below still has the two hand-written operators, and each
// has rotted in its own way: one destroys its own contents on
// self-assignment (no sanitizer speaks — the assert after `b = alias`
// finds zeros where the data was), the other forgets its old buffer
// (the `live` counter knows).
//
// Task: replace BOTH assignment operators with ONE, copy-and-swap style.
//   - every assert passes and the run is sanitizer-clean
//   - IntBuffer::live is 0 at the end: every buffer freed exactly once
//   - move assignment still STEALS (pointer identity) and still cannot
//     throw — the static_assert insists
// Constraints:
//   - exactly one operator= remains, and it has no self-assignment
//     check: the idiom must not need one
//   - write the swap as a noexcept hidden friend of IntBuffer
//   - the constructors, the destructor, grab/release, and main stay
//     as they are

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

class IntBuffer {
public:
    inline static int live = 0;     // buffers currently allocated

    explicit IntBuffer(std::size_t n) : n_(n), data_(grab(n)) {}
    ~IntBuffer() { release(data_); }

    IntBuffer(const IntBuffer& other) : n_(other.n_), data_(grab(other.n_)) {
        for (std::size_t i = 0; i < n_; ++i) data_[i] = other.data_[i];
    }
    IntBuffer(IntBuffer&& other) noexcept
        : n_(std::exchange(other.n_, 0)),
          data_(std::exchange(other.data_, nullptr)) {}

    // Hand-written copy assignment. The order is wrong for ONE caller...
    IntBuffer& operator=(const IntBuffer& other) {
        release(data_);
        n_ = other.n_;
        data_ = grab(n_);
        for (std::size_t i = 0; i < n_; ++i) data_[i] = other.data_[i];
        return *this;
    }

    // Hand-written move assignment. Steals nicely. Forgets something.
    IntBuffer& operator=(IntBuffer&& other) noexcept {
        n_ = std::exchange(other.n_, 0);
        data_ = std::exchange(other.data_, nullptr);
        return *this;
    }

    const int* data() const { return data_; }
    int& operator[](std::size_t i) { return data_[i]; }
    int operator[](std::size_t i) const { return data_[i]; }
    std::size_t size() const { return n_; }

private:
    // Every allocation and every free goes through these two, so `live`
    // is an honest count.
    static int* grab(std::size_t n) { ++live; return new int[n](); }
    static void release(int* p) { if (p) { --live; delete[] p; } }

    std::size_t n_;
    int* data_;
};

int main() {
    {
        IntBuffer a(3);
        a[0] = 10; a[1] = 20; a[2] = 30;
        IntBuffer b(2);

        b = a;                           // an lvalue: must COPY
        assert(b.size() == 3 && b[2] == 30);
        b[0] = 99;
        assert(a[0] == 10);              // deep copy: a is untouched

        IntBuffer& alias = b;
        b = alias;                       // self-assignment in disguise
        assert(b.size() == 3 && b[0] == 99 && b[2] == 30);

        IntBuffer c(1);
        const int* guts = b.data();
        c = std::move(b);                // an rvalue: must STEAL
        assert(c.data() == guts);        // pointer identity = real theft
        assert(c.size() == 3 && c[0] == 99);
    }
    assert(IntBuffer::live == 0);        // nothing leaked, nothing freed twice

    static_assert(std::is_nothrow_move_assignable_v<IntBuffer>,
                  "assigning from an rvalue must not be able to throw");
    return 0;
}

// I AM NOT DONE
