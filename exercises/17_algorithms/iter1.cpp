// iter1.cpp
//
// Every algorithm in this module took a PAIR OF ITERATORS and never
// asked what container they came from. That is the whole design: the
// algorithms are written against a small interface, and anything that
// offers the interface gets all of them — including a container the
// standard library has never heard of.
//
// The interface, for the simplest useful kind (a FORWARD iterator):
//
//     *it          the element it refers to
//     ++it, it++   step to the next element
//     a == b       do two iterators refer to the same position?
//
// plus two member type names the library reads to learn what it is
// dealing with (value_type, difference_type), and the ability to be
// default-constructed and copied. That's it. A raw pointer into an
// array already satisfies all of it — pointers are the original
// iterators; the abstraction was modeled on them.
//
// A container then offers begin() (the first element) and end() (ONE
// PAST the last — never dereferenced, only compared against). With
// those two members a type works in a range-for loop too: the loop is
// just `for (auto it = begin(); it != end(); ++it)` in a short coat.
//
// C++20 turned the informal interface into a concept you can ask
// about: std::forward_iterator<T>. This file does — in a
// static_assert — so the compiler tells you exactly which requirement
// is still missing. Read its complaint; it is a to-do list.
//
// Window below is a small ring (ring1's shape: fixed storage, a head
// index, a count). Its logical order starts at head_ and WRAPS. It has
// begin() and end() and an iterator type with the right data — the
// iterator just cannot DO anything yet.
//
// Task: finish Window::iterator so the standard algorithms accept it.
//   - the static_assert holds, and every assert passes
//   - iteration follows the LOGICAL order (oldest first, wrapping
//     through the end of the array), not the storage order
// Constraints:
//   - add the missing operations to Window::iterator only — its two
//     data members are all the state it needs
//   - don't change push(), begin(), end(), main, or the asserts
//   - no copying the elements out into another container

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

        const Window* window = nullptr;   // the ring we walk
        std::size_t offset = 0;           // steps from the OLDEST element

        // TODO: the operations from the list above.
    };

    iterator begin() const { return {this, 0}; }
    iterator end() const { return {this, count_}; }

    // Newest element in; when full, the oldest is overwritten.
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
    std::size_t head_ = 0;     // index of the oldest element
    std::size_t count_ = 0;    // number of live elements
};

static_assert(std::forward_iterator<Window<int, 4>::iterator>,
              "Window::iterator must model std::forward_iterator");

int main() {
    Window<int, 4> w;
    for (int v : {1, 2, 3, 4, 5, 6}) {
        w.push(v);                 // 1 and 2 are overwritten: the ring
    }                              // now holds 3 4 5 6, stored as 5 6 3 4

    // A range-for needs nothing but begin(), end(), !=, ++ and *:
    int digits = 0;
    for (int v : w) {
        digits = digits * 10 + v;
    }
    assert(digits == 3456);        // LOGICAL order, oldest first

    // ...and so do the algorithms:
    assert(std::accumulate(w.begin(), w.end(), 0) == 18);
    assert(*std::max_element(w.begin(), w.end()) == 6);
    assert(std::find(w.begin(), w.end(), 5) != w.end());
    assert(std::find(w.begin(), w.end(), 2) == w.end());   // overwritten
    assert(std::distance(w.begin(), w.end()) == 4);
    return 0;
}

// I AM NOT DONE
