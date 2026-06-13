// extras5.cpp  — GRADUATION QUIZ
//
// The last exercise. Every TODO below is a flagship fact from one
// module of this course — if one stumps you, the comment names where
// to look. Predict all, run once, graduate.
//
// (And the two reading notes to leave with:
//   MODULES (C++20): `import std;` replaces textual #include — the
//   whole module-06 problem class, solved at the root. Adopt when your
//   build system is ready.
//   C++23: std::print, std::expected, std::generator, deducing this —
//   the next vintage. You now know how to read its release notes.)

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

constexpr int TODO = -1;
constexpr bool TODO_BOOL = false;

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
    // Module 01 — brace init refuses narrowing; old init doesn't:
    double price = 3.9;
    int silent = price;
    assert(silent == TODO);

    // Module 01 — integral promotion:
    static_assert(std::is_same_v<decltype('a' + 1), int> == TODO_BOOL);

    // Module 09/21 — guaranteed elision: how many copies?
    Tracker::copies = 0;
    Tracker t = make_tracker();
    assert(Tracker::copies == TODO);

    // Module 09 — std::move on a CONST object:
    const Tracker frozen;
    Tracker::copies = 0;
    Tracker thief = std::move(frozen);
    assert(Tracker::copies == TODO);

    // Module 10 — ownership at zero cost:
    static_assert((sizeof(std::unique_ptr<int>) == sizeof(int*)) == TODO_BOOL);

    // Module 15 — SSO: does this allocate?
    // Module 16 — does the loop's push_back invalidate `first`?
    std::vector<int> v;
    v.reserve(3);
    v.push_back(1);
    int* first = &v[0];
    v.push_back(2);
    v.push_back(3);
    bool still_valid = (first == &v[0]);     // reserved capacity!
    assert(still_valid == TODO_BOOL);

    // Module 19 — what does the unchecked accessor on an empty
    // optional do? (Don't answer with code. Just remember.)

    // Module 22 — and the meta-lesson: if you're unsure whether
    // something is UB... the sanitizers are already in your flags.

    (void)t; (void)thief;
    return 0;                                // — fin. Now go read a
}                                            // real codebase. 🎓

// I AM NOT DONE
