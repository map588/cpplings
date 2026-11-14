// swap1.cpp — solution

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

class IntBuffer {
public:
    inline static int live = 0;

    explicit IntBuffer(std::size_t n) : n_(n), data_(grab(n)) {}
    ~IntBuffer() { release(data_); }

    IntBuffer(const IntBuffer& other) : n_(other.n_), data_(grab(other.n_)) {
        for (std::size_t i = 0; i < n_; ++i) data_[i] = other.data_[i];
    }
    IntBuffer(IntBuffer&& other) noexcept
        : n_(std::exchange(other.n_, 0)),
          data_(std::exchange(other.data_, nullptr)) {}

    // ONE assignment operator. `other` arrives already built: by the copy
    // constructor for an lvalue argument, by the move constructor for an
    // rvalue. A copy that fails throws at the call site, before this
    // function starts — so the function itself can be noexcept.
    IntBuffer& operator=(IntBuffer other) noexcept {
        swap(*this, other);
        return *this;
    }   // `other` dies here, holding our old buffer: its destructor frees it

    // Hidden friend: found by ADL only. Trading two members cannot fail.
    friend void swap(IntBuffer& a, IntBuffer& b) noexcept {
        using std::swap;
        swap(a.n_, b.n_);
        swap(a.data_, b.data_);
    }

    const int* data() const { return data_; }
    int& operator[](std::size_t i) { return data_[i]; }
    int operator[](std::size_t i) const { return data_[i]; }
    std::size_t size() const { return n_; }

private:
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

        b = a;
        assert(b.size() == 3 && b[2] == 30);
        b[0] = 99;
        assert(a[0] == 10);

        IntBuffer& alias = b;
        b = alias;                       // copies first, then swaps: harmless
        assert(b.size() == 3 && b[0] == 99 && b[2] == 30);

        IntBuffer c(1);
        const int* guts = b.data();
        c = std::move(b);                // move-constructs `other`, then swaps
        assert(c.data() == guts);
        assert(c.size() == 3 && c[0] == 99);
    }
    assert(IntBuffer::live == 0);

    static_assert(std::is_nothrow_move_assignable_v<IntBuffer>,
                  "assigning from an rvalue must not be able to throw");
    return 0;
}
