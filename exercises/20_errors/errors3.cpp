// errors3.cpp
//
// noexcept is not documentation — it's an enforced promise. An
// exception trying to LEAVE a noexcept function doesn't propagate:
// the runtime calls std::terminate. No unwinding to the caller, no
// catch, no cleanup above this frame. Run this as-is: the try/catch
// in main never gets its chance.
//
// Someone marked parse_quantity noexcept ("it's just parsing!") — but
// std::stoi throws std::invalid_argument on garbage. The keyword turned
// a recoverable error into process death.
//
// When TO write noexcept:
//   - move constructors / move assignment (module 09: vector relies
//     on it — this is the big one)
//   - swap functions, and other "cannot fail by construction" code
//   - destructors are implicitly noexcept — don't throw in them
//
// When NOT to: any function whose callees may throw, unless you catch
// everything inside. "Probably doesn't throw" is not noexcept.
//
// The compile-time query operator (same keyword, different role):
//     static_assert(!noexcept(parse_quantity("")));   // "may it throw?"
//
// Task: fix the promise (don't fix stoi — garbage input is a legitimate
// runtime condition here). Make the static_assert at the bottom agree.

// I AM NOT DONE

#include <cassert>
#include <string>

int parse_quantity(const std::string& text) noexcept {
    return std::stoi(text);
}

int main() {
    assert(parse_quantity("12") == 12);

    int qty = -1;
    try {
        qty = parse_quantity("twelve");
    } catch (const std::exception&) {
        qty = 0;                       // graceful: unknown quantity
    }
    assert(qty == 0);

    // The promise, queried at compile time — uncomment once fixed:
    // static_assert(!noexcept(parse_quantity(std::string{})),
    //               "parse_quantity may throw — it must not claim otherwise");
    return 0;
}
