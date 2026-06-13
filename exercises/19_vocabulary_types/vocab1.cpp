// vocab1.cpp  — QUIZ for module 19, plus the corner pieces
//
// std::any (C++17): a box for a value of ANY type — variant without the
// guest list. The type check happens at RUNTIME:
//
//     std::any box = 42;
//     std::any_cast<int>(box)       // 42
//     std::any_cast<bool>(box)      // throws std::bad_any_cast
//     std::any_cast<int>(&box)      // pointer form: nullptr on mismatch
//     box.has_value() / box.type()  // inspection
//
// Honest guidance: any is the LAST resort. A known set of types →
// variant (compile-time exhaustiveness). One type, maybe absent →
// optional. any is for genuinely open-ended plumbing (script bindings,
// property bags) — and every any_cast is a runtime gamble the compiler
// can't vet.
//
// READING NOTE — std::expected<T, E> (C++23): the missing sibling.
// optional says "maybe absent"; expected says "value, OR THE REASON
// there isn't one": parse_port (strings4!) returning
// expected<int, ParseError> beats both -1 and exceptions for everyday
// fallible calls. Until C++23 is your floor, the pattern is faked with
// variant<T, E> — which you now know how to visit.
//
// Task: fill the TODOs. Mind #3: any_cast checks the EXACT type — the
// implicit-conversion family (modules 01/04) does not apply.

#include <any>
#include <cassert>
#include <string>

constexpr bool TODO_BOOL = false;

int main() {
    std::any box;
    assert(box.has_value() == TODO_BOOL);            // 1. fresh any

    box = std::string("payload");
    box = 42;                                        // replaces the string
    assert(box.has_value() == TODO_BOOL);            // 2.

    // 3. It holds the int 42. Does the LONG cast succeed?
    bool long_cast_ok = (std::any_cast<long>(&box) != nullptr);
    assert(long_cast_ok == TODO_BOOL);

    // 4. The pointer form never throws; the value form...
    bool threw = false;
    try {
        auto s = std::any_cast<std::string>(box);    // it's an int now
        (void)s;
    } catch (const std::bad_any_cast&) {
        threw = true;
    }
    assert(threw == TODO_BOOL);

    // 5. The right cast, for closure:
    assert(std::any_cast<int>(box) == 42);
    return 0;
}

// I AM NOT DONE
