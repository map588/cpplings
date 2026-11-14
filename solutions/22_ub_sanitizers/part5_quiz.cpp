// part5_quiz.cpp — solution

#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

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
    static_assert(noexcept(may_throw()) == false);
    static_assert(noexcept(never_throws()) == true);
    // ...and the destructor nobody annotated?
    static_assert(std::is_nothrow_destructible_v<Plain> == true);

    // Module 20 — handlers are tried top to bottom:
    assert(first_match() == 'l');

    // Module 20 — unwinding: who is destroyed first, and does that
    // happen before or after the handler runs?
    assert(unwind_log() == "ba!");

    // Module 21 — decltype((expr)): T& = lvalue, T&& = xvalue, T = prvalue.
    int x = 0;
    int arr[2] = {};
    static_assert(std::is_lvalue_reference_v<decltype((x))> == true);
    static_assert(std::is_lvalue_reference_v<decltype((arr[1]))> == true);
    static_assert(std::is_reference_v<decltype((x + 1))> == false);
    static_assert(std::is_rvalue_reference_v<decltype((std::move(x)))> == true);
    (void)x; (void)arr;

    // Module 21 — a return type decides the category of the CALL:
    static_assert(std::is_reference_v<decltype((by_value()))> == false);
    static_assert(std::is_lvalue_reference_v<decltype((by_ref()))> == true);
    static_assert(std::is_rvalue_reference_v<decltype((by_rref()))> == true);

    // Module 22 — the DEFINED side of the line.
    // Unsigned arithmetic wraps. Is UINT_MAX + 1 equal to 0?
    static_assert((std::numeric_limits<unsigned>::max() + 1u == 0u) == true);
    // Integer division truncates toward zero:
    static_assert(-7 / 2 == -3);
    static_assert(-7 % 4 == -3);
    // Since C++20 a signed left shift wraps. Is 1 << 31 negative?
    static_assert(((1 << 31) < 0) == true);
    // Floating-point division by zero follows IEEE 754. Is the result
    // an infinity?
    volatile double zero = 0.0;
    assert(std::isinf(1.0 / zero) == true);

    // Module 22 — the checked door next to the unchecked one:
    std::vector<int> v(3);
    bool threw = false;
    try {
        (void)v.at(3);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw == true);
    return 0;
}
