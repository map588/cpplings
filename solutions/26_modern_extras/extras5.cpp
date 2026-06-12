// extras5.cpp — solution

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

struct Tracker {
    inline static int copies = 0;
    Tracker() = default;
    Tracker(const Tracker&) { ++copies; }
    Tracker(Tracker&&) noexcept = default;
    Tracker& operator=(const Tracker&) = default;
    Tracker& operator=(Tracker&&) noexcept = default;
};

Tracker make_tracker() { return Tracker{}; }

int main() {
    // 3.9 silently truncates (int silent{3.9} would not compile):
    double price = 3.9;
    int silent = price;
    assert(silent == 3);

    // char + int promotes to int: true.
    static_assert(std::is_same_v<decltype('a' + 1), int> == true);

    // Guaranteed elision (C++17): zero copies, zero moves.
    Tracker::copies = 0;
    Tracker t = make_tracker();
    assert(Tracker::copies == 0);

    // const defeats move: the copy ctor wins, silently. 1 copy.
    const Tracker frozen;
    Tracker::copies = 0;
    Tracker thief = std::move(frozen);
    assert(Tracker::copies == 1);

    // unique_ptr with default deleter is pointer-sized: true.
    static_assert((sizeof(std::unique_ptr<int>) == sizeof(int*)) == true);

    // reserve(3) means no reallocation across three pushes: true.
    std::vector<int> v;
    v.reserve(3);
    v.push_back(1);
    int* first = &v[0];
    v.push_back(2);
    v.push_back(3);
    bool still_valid = (first == &v[0]);
    assert(still_valid == true);

    (void)t; (void)thief;
    return 0;   // 🎓
}
