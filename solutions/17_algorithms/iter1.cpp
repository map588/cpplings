// iter1.cpp — solution

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <numeric>

template <typename T, std::size_t N>
class Window {
public:
    struct iterator {
        using value_type = T;
        using difference_type = std::ptrdiff_t;

        const Window* window = nullptr;
        std::size_t offset = 0;

        // The element `offset` steps after the oldest one, wrapped.
        // (A nested class may read the enclosing class's privates.)
        const T& operator*() const {
            return window->buf_[(window->head_ + offset) % N];
        }
        iterator& operator++() {          // ++it
            ++offset;
            return *this;
        }
        iterator operator++(int) {        // it++: step, return the old one
            iterator old = *this;
            ++offset;
            return old;
        }
        // Same ring, same offset. (!= is derived from == since C++20.)
        bool operator==(const iterator&) const = default;
    };

    iterator begin() const { return {this, 0}; }
    iterator end() const { return {this, count_}; }

    void push(const T& v) {
        buf_[(head_ + count_) % N] = v;
        if (count_ == N) {
            head_ = (head_ + 1) % N;
        } else {
            ++count_;
        }
    }

private:
    std::array<T, N> buf_{};
    std::size_t head_ = 0;
    std::size_t count_ = 0;
};

static_assert(std::forward_iterator<Window<int, 4>::iterator>,
              "Window::iterator must model std::forward_iterator");

int main() {
    Window<int, 4> w;
    for (int v : {1, 2, 3, 4, 5, 6}) {
        w.push(v);
    }

    int digits = 0;
    for (int v : w) {
        digits = digits * 10 + v;
    }
    assert(digits == 3456);

    assert(std::accumulate(w.begin(), w.end(), 0) == 18);
    assert(*std::max_element(w.begin(), w.end()) == 6);
    assert(std::find(w.begin(), w.end(), 5) != w.end());
    assert(std::find(w.begin(), w.end(), 2) == w.end());
    assert(std::distance(w.begin(), w.end()) == 4);
    return 0;
}
