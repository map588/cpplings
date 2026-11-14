// part3_quiz.cpp  — QUIZ for Part III (modules 12–14)
//
// Generic programming, end to end: templates, the closures that feed
// them, and the concepts that keep them honest. Every probe below is a
// fact from module 12, 13 or 14 — the comment names which.
//
// Task: replace every TODO, TODO_SIZE, TODO_BOOL and TODO_C with your
// prediction.
//   - the program compiles and every assert and static_assert passes
// Constraints:
//   - change only the TODO values — the probes stay as they are
//   - predict first, compile second (sizes assume this course's
//     platforms: int is 4 bytes)

#include <cassert>
#include <concepts>
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

constexpr int TODO = -1;
constexpr std::size_t TODO_SIZE = 9999;
constexpr bool TODO_BOOL = false;
constexpr char TODO_C = '?';

// ---- module 12: a class template and one full specialization ---------
template <typename T>
struct Kind { static constexpr char tag = 'g'; };     // the generic recipe

template <>
struct Kind<bool> { static constexpr char tag = 'b'; };

// ---- module 12: an NTTP deduced from an array --------------------------
template <typename T, std::size_t N>
constexpr std::size_t length(T (&)[N]) { return N; }

// ---- module 14: constrained overloads ----------------------------------
template <typename T>
constexpr char describe(T) { return 'x'; }
constexpr char describe(std::integral auto) { return 'i'; }
constexpr char describe(std::floating_point auto) { return 'f'; }

// ---- module 14: folds ---------------------------------------------------
template <typename... Ts>
constexpr int total(Ts... xs) { return (0 + ... + xs); }

template <typename... Ts>
constexpr bool all_true(Ts... xs) { return (xs && ...); }

// ---- module 14: if constexpr --------------------------------------------
template <typename T>
constexpr int indirections(T) {
    if constexpr (std::is_pointer_v<T>) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    // Module 12 — which recipe does each instantiation use?
    static_assert(Kind<int>::tag == TODO_C);
    static_assert(Kind<bool>::tag == TODO_C);

    // Module 12 — the array's length rides in as a template argument:
    int grid[7] = {};
    static_assert(length(grid) == TODO_SIZE);
    (void)grid;

    // Module 13 — a capture happens when the lambda is CREATED:
    int n = 1;
    auto by_value = [n] { return n; };
    auto by_ref = [&n] { return n; };
    n = 5;
    assert(by_value() == TODO);
    assert(by_ref() == TODO);

    // Module 13 — a closure is a struct: you pay for what you capture.
    auto nothing = [] { return 0; };
    auto one_int = [n] { return n; };
    static_assert(sizeof(nothing) == TODO_SIZE);
    static_assert(sizeof(one_int) == TODO_SIZE);

    // Module 13 — and only one of those two converts to a plain
    // function pointer:
    static_assert(std::is_convertible_v<decltype(nothing), int (*)()> == TODO_BOOL);
    static_assert(std::is_convertible_v<decltype(one_int), int (*)()> == TODO_BOOL);
    (void)nothing; (void)one_int;

    // Module 13 — copying a closure copies its state:
    auto next = [i = 0]() mutable { return ++i; };
    next();
    next();
    auto twin = next;
    assert(twin() == TODO);
    assert(next() == TODO);

    // Module 14 — a concept is a compile-time predicate:
    static_assert(std::integral<bool> == TODO_BOOL);
    static_assert(std::floating_point<int> == TODO_BOOL);
    static_assert(std::convertible_to<const char*, std::string> == TODO_BOOL);
    static_assert(std::same_as<int, const int> == TODO_BOOL);

    // Module 14 — the most constrained overload wins:
    static_assert(describe(42) == TODO_C);
    static_assert(describe(2.5f) == TODO_C);
    static_assert(describe("hi") == TODO_C);

    // Module 14 — folds, the empty pack included:
    static_assert(total(1, 2, 3) == TODO);
    static_assert(total() == TODO);
    static_assert(all_true(true, false) == TODO_BOOL);
    static_assert(all_true() == TODO_BOOL);

    // Module 14 — CTAD: what did the compiler deduce?
    std::pair p{1, 2.5};
    std::vector v{1.5, 2.5};
    static_assert(std::is_same_v<decltype(p), std::pair<int, double>> == TODO_BOOL);
    static_assert(std::is_same_v<decltype(v), std::vector<double>> == TODO_BOOL);
    (void)p; (void)v;

    // Module 14 — if constexpr keeps one branch per instantiation:
    static_assert(indirections("text") == TODO);
    static_assert(indirections(42) == TODO);
    return 0;
}

// I AM NOT DONE
