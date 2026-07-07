// move2.cpp
//
// copy1 (module 08) made you write the rule of three for IntBuffer.
// Completing it to the RULE OF FIVE means adding the two move members.
// What each one owes:
//
//   move ctor:    transfer ownership of the guts; the source must be
//                 left safe to destroy (and to assign a new value to)
//   move assign:  same — but remember YOU already own a buffer, and
//                 nothing stops a self-move
//   both:         must promise they can't throw. The promise is honest:
//                 stealing never allocates, and only allocation can
//                 fail. (Why the promise MATTERS: move3. The
//                 static_assert below insists on it.)
//
// The asserts below verify actual theft, pointer-identically, just like
// move1 did for std::string.
//
// Task: implement the move constructor and move assignment.
//   - compiles (static_assert included) and every assert passes
//   - sanitizer-clean: no leak, no double-free — moved-from objects
//     still get destroyed
//   - moves must steal (pointer identity), not copy elements
// Constraints:
//   - the copy operations and destructor stay exactly as they are
//   - don't change main or the asserts

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

    // Rule of five — the missing two go here.

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
