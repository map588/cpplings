// copy1.cpp — solution

#include <cassert>
#include <cstddef>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t n) : n_(n), data_(new int[n]()) {}

    ~IntBuffer() { delete[] data_; }

    IntBuffer(const IntBuffer& other)
        : n_(other.n_), data_(new int[other.n_]) {
        for (std::size_t i = 0; i < n_; ++i) data_[i] = other.data_[i];
    }

    IntBuffer& operator=(const IntBuffer& other) {
        if (this == &other) return *this;   // 1. self-assignment
        delete[] data_;                     // 2. release the old buffer
        n_ = other.n_;                      // 3. allocate + copy
        data_ = new int[n_];
        for (std::size_t i = 0; i < n_; ++i) data_[i] = other.data_[i];
        return *this;
    }

    int& operator[](std::size_t i) { return data_[i]; }
    int operator[](std::size_t i) const { return data_[i]; }
    std::size_t size() const { return n_; }

private:
    std::size_t n_;
    int* data_;
};

int main() {
    IntBuffer a(3);
    a[0] = 10; a[1] = 20; a[2] = 30;

    IntBuffer b = a;
    b[0] = 99;
    assert(a[0] == 10);

    IntBuffer c(1);
    c = a;
    assert(c.size() == 3 && c[2] == 30);

    IntBuffer& alias = c;     // self-assignment in disguise (the compiler
    c = alias;                // warns on a literal c = c) — must be harmless
    assert(c[2] == 30);

    return 0;
}
