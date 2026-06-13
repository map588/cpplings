// move2.cpp
//
// copy1 (module 08) made you write the rule of three for IntBuffer.
// Completing it to the RULE OF FIVE means adding the two move members,
// and the recipe is always the same:
//
//   move ctor:    STEAL the guts, then NULL the source's pointers
//                 (so its destructor deletes nullptr — a harmless no-op)
//   move assign:  guard self-move, free your own guts, steal, null
//   both:         mark noexcept — moves that can't throw. Stealing a
//                 pointer can't fail; only allocating can, and moves
//                 don't allocate. (Why noexcept MATTERS: move3.)
//
// The asserts below verify actual theft, pointer-identically, just like
// move1 did for std::string.
//
// Task: implement the move constructor and move assignment.

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t n) : n_(n), data_(new int[n]()) {}
    ~IntBuffer() { delete[] data_; }

    IntBuffer(const IntBuffer& other)
        : n_(other.n_), data_(new int[other.n_]) {
        for (std::size_t i = 0; i < n_; ++i) data_[i] = other.data_[i];
    }
    IntBuffer& operator=(const IntBuffer& other) {
        if (this == &other) return *this;
        delete[] data_;
        n_ = other.n_;
        data_ = new int[n_];
        for (std::size_t i = 0; i < n_; ++i) data_[i] = other.data_[i];
        return *this;
    }

    // Rule of five — the missing two go here:
    // IntBuffer(IntBuffer&& other) noexcept ...
    // IntBuffer& operator=(IntBuffer&& other) noexcept ...

    const int* data() const { return data_; }
    int& operator[](std::size_t i) { return data_[i]; }
    std::size_t size() const { return n_; }

private:
    std::size_t n_;
    int* data_;
};

int main() {
    IntBuffer a(3);
    a[0] = 7;
    const int* buf = a.data();

    IntBuffer b = std::move(a);            // must STEAL, not copy
    assert(b.data() == buf);               // pointer identity = real theft
    assert(b.size() == 3 && b[0] == 7);

    IntBuffer c(1);
    c = std::move(b);                      // move assignment too
    assert(c.data() == buf);

    static_assert(std::is_nothrow_move_constructible_v<IntBuffer>,
                  "move members must be noexcept (move3 explains why)");
    return 0;
}

// I AM NOT DONE
