// move6.cpp
//
// In a TEMPLATE, `T&&` is not an rvalue reference. When T is being
// DEDUCED, T&& is a FORWARDING REFERENCE — it binds to anything:
//
//     template <typename T> void f(T&& x);
//     f(name);          // lvalue  → T = std::string&,  x: std::string&
//     f(get_name());    // rvalue  → T = std::string,   x: std::string&&
//
// (Reference collapsing makes this work: & + && = &, && + && = &&.
// And only DEDUCED T&& forwards — std::string&& is just an rvalue ref.)
//
// Inside f, x has a NAME, so it's an lvalue (module 04!) — passing it on
// would copy. The naive "fix" is std::move(x)... which also plunders the
// CALLER'S OBJECT when the caller passed an lvalue. That's the shipped
// bug: store(label) quietly guts `label`.
//
//     std::forward<T>(x)     // the real tool (C++11): casts x back to
//                            // whatever the caller passed — lvalues stay
//                            // lvalues (copied), rvalues become rvalues
//                            // (moved). "Conditional move."
//
// Rule of thumb: std::move on T&& where T is concrete; std::forward<T>
// on T&& where T was deduced. Never both, never the other way.
//
// Task: one word to change.

#include <cassert>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> vault;

template <typename T>
void store(T&& value) {
    vault.push_back(std::move(value));   // moves even from lvalues!
}

int main() {
    std::string label = "a label far too long for the small-string "
                        "optimization, so theft is observable";
    std::string original = label;

    store(label);                    // lvalue in: caller keeps their string
    assert(label == original);       // ...unless someone moved it

    store(std::move(label));         // rvalue in: NOW it should move
    assert(vault.size() == 2);
    assert(vault[0] == original && vault[1] == original);
    return 0;
}

// I AM NOT DONE
