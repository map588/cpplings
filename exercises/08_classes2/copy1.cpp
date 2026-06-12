// copy1.cpp
//
// The compiler-generated copy constructor and copy assignment copy each
// member. For an int* member that means copying THE POINTER — and now two
// objects both believe they own one allocation. Both destructors run,
// both delete[]: AddressSanitizer reports a double-free. Run it and look.
//
// This class manages a resource by hand (new[]/delete[]), so member-wise
// copying is wrong — and the RULE OF THREE says: a class that needs ANY
// of {destructor, copy ctor, copy assignment} needs ALL THREE.
//
// Task: write the missing two. The copy constructor allocates its own
// buffer and copies the contents. Copy assignment is trickiest — the
// classic direct version must handle, in order:
//     1. self-assignment (b = b must not free the buffer it's copying!)
//     2. free the OLD buffer
//     3. allocate + copy the new contents
// (Module 09 shows the copy-and-swap idiom that gets this for free.)
//
// The honest fix is a std::vector member — that's copy2. Do it by hand
// exactly once, here, so you know what the rule of three costs.

// I AM NOT DONE

#include <cassert>
#include <cstddef>

class IntBuffer {
public:
    explicit IntBuffer(std::size_t n) : n_(n), data_(new int[n]()) {}

    ~IntBuffer() { delete[] data_; }

    // Rule of three: the other two go here.

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

    IntBuffer b = a;          // member-wise copy: b.data_ == a.data_ !
    b[0] = 99;
    assert(a[0] == 10);       // distinct buffers? not yet...

    IntBuffer c(1);
    c = a;                    // copy assignment: same disease
    assert(c.size() == 3 && c[2] == 30);

    IntBuffer& alias = c;     // self-assignment in disguise (the compiler
    c = alias;                // warns on a literal c = c) — must be harmless
    assert(c[2] == 30);

    return 0;                 // ...and here both a and b delete[] the
}                             // same pointer: double-free
