// move2.cpp — solution

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

    // Steal, then null the source.
    IntBuffer(IntBuffer&& other) noexcept
        : n_(other.n_), data_(other.data_) {
        other.n_ = 0;
        other.data_ = nullptr;       // its dtor now deletes nullptr: no-op
    }

    IntBuffer& operator=(IntBuffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] data_;              // free our own guts first
        n_ = other.n_;
        data_ = other.data_;
        other.n_ = 0;
        other.data_ = nullptr;
        return *this;
    }

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

    IntBuffer b = std::move(a);
    assert(b.data() == buf);
    assert(b.size() == 3 && b[0] == 7);

    IntBuffer c(1);
    c = std::move(b);
    assert(c.data() == buf);

    static_assert(std::is_nothrow_move_constructible_v<IntBuffer>,
                  "move members must be noexcept (move3 explains why)");
    return 0;
}
