// span1.cpp
//
// Three callers, three argument shapes — vector, std::array, C array —
// and the 2009 solution was three overloads or the lowest common
// denominator, (const int*, size_t). Easy to pass the wrong length;
// nothing stops sum(ptr, 999).
//
// std::span<const int> (C++20) is the one-signature answer: a
// {pointer, size} view over ANY contiguous int storage. It's
// string_view's generalization (module 15) — non-owning, cheap to copy,
// pass by value. Same lifetime rules too: a span doesn't keep anything
// alive, and growth of the viewed vector invalidates it (vector1!).
//
//   span<const int>   read-only view  ← parameters, default
//   span<int>         writable view   ← when you mutate the elements
//   span<int, 4>      compile-time extent (rarer; NTTP, module 12)
//
// Task: collapse the three sum_* functions into one sum(std::span<const
// int>), and update the calls.

#include <array>
#include <cassert>
#include <cstddef>
#include <span>
#include <vector>

int sum_vector(const std::vector<int>& xs) {
    int total = 0;
    for (int x : xs) total += x;
    return total;
}

int sum_array(const std::array<int, 4>& xs) {
    int total = 0;
    for (int x : xs) total += x;
    return total;
}

int sum_carray(const int* xs, std::size_t n) {
    int total = 0;
    for (std::size_t i = 0; i < n; ++i) total += xs[i];
    return total;
}

int main() {
    std::vector<int> v = {1, 2, 3};
    std::array<int, 4> a = {10, 20, 30, 40};
    int c[] = {5, 5};

    assert(sum(v) == 6);                  // one function...
    assert(sum(a) == 100);                // ...all three callers
    assert(sum(c) == 10);

    // Subranges come free with a view:
    assert(sum(std::span<const int>(a).subspan(1, 2)) == 50);
    return 0;
}

// I AM NOT DONE
