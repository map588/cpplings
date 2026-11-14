// part5_quiz.cpp  — QUIZ for Part V (modules 20–22)
//
// Correctness, in three questions: what happens when things go wrong
// (module 20), what KIND of thing an expression is (module 21), and
// where the language stops promising anything (module 22). This quiz
// only probes behavior that IS defined — the undefined kind you met
// under the sanitizers, and a quiz must not go back there.
//
// Task: replace every TODO, TODO_BOOL, TODO_C and TODO_S with your
// prediction.
//   - the program compiles and every assert and static_assert passes
// Constraints:
//   - change only the TODO values — the probes stay as they are
//   - predict first, compile second

#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

constexpr int TODO = -1;
constexpr bool TODO_BOOL = false;
constexpr char TODO_C = '?';
const std::string TODO_S = "?";

// ---- module 20 ----------------------------------------------------------
void may_throw();
void never_throws() noexcept;

struct Plain {
    ~Plain() {}                 // a destructor nobody annotated
};

// std::out_of_range derives from std::logic_error, which derives from
// std::exception. Which handler runs?
char first_match() {
    try {
        throw std::out_of_range("index 9 of 3");
    } catch (const std::logic_error&) {
        return 'l';
    } catch (const std::exception&) {
        return 'e';
    }
}

// Each Probe appends its tag when it is destroyed.
struct Probe {
    char tag;
    std::string& log;
    ~Probe() { log += tag; }
};

std::string unwind_log() {
    std::string log;
    try {
        Probe a{'a', log};
        Probe b{'b', log};
        throw std::runtime_error("boom");
    } catch (const std::runtime_error&) {
        log += '!';
    }
    return log;
}

// ---- module 21 ----------------------------------------------------------
int   by_value();
int&  by_ref();
int&& by_rref();

int main() {
    // Module 20 — the noexcept OPERATOR asks at compile time:
    static_assert(noexcept(may_throw()) == TODO_BOOL);
    static_assert(noexcept(never_throws()) == TODO_BOOL);
    // ...and the destructor nobody annotated?
    static_assert(std::is_nothrow_destructible_v<Plain> == TODO_BOOL);

    // Module 20 — handlers are tried top to bottom:
    assert(first_match() == TODO_C);

    // Module 20 — unwinding: who is destroyed first, and does that
    // happen before or after the handler runs?
    assert(unwind_log() == TODO_S);

    // Module 21 — decltype((expr)): T& = lvalue, T&& = xvalue, T = prvalue.
    int x = 0;
    int arr[2] = {};
    static_assert(std::is_lvalue_reference_v<decltype((x))> == TODO_BOOL);
    static_assert(std::is_lvalue_reference_v<decltype((arr[1]))> == TODO_BOOL);
    static_assert(std::is_reference_v<decltype((x + 1))> == TODO_BOOL);
    static_assert(std::is_rvalue_reference_v<decltype((std::move(x)))> == TODO_BOOL);
    (void)x; (void)arr;

    // Module 21 — a return type decides the category of the CALL:
    static_assert(std::is_reference_v<decltype((by_value()))> == TODO_BOOL);
    static_assert(std::is_lvalue_reference_v<decltype((by_ref()))> == TODO_BOOL);
    static_assert(std::is_rvalue_reference_v<decltype((by_rref()))> == TODO_BOOL);

    // Module 22 — the DEFINED side of the line.
    // Unsigned arithmetic wraps. Is UINT_MAX + 1 equal to 0?
    static_assert((std::numeric_limits<unsigned>::max() + 1u == 0u) == TODO_BOOL);
    // Integer division truncates toward zero:
    static_assert(-7 / 2 == TODO);
    static_assert(-7 % 4 == TODO);
    // Since C++20 a signed left shift wraps. Is 1 << 31 negative?
    static_assert(((1 << 31) < 0) == TODO_BOOL);
    // Floating-point division by zero follows IEEE 754. Is the result
    // an infinity?
    volatile double zero = 0.0;
    assert(std::isinf(1.0 / zero) == TODO_BOOL);

    // Module 22 — the checked door next to the unchecked one:
    std::vector<int> v(3);
    bool threw = false;
    try {
        (void)v.at(3);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw == TODO_BOOL);
    return 0;
}

// I AM NOT DONE
