// move4.cpp
//
// The fastest move is no move. COPY ELISION: the compiler constructs the
// returned object DIRECTLY in the caller's storage — no copy, no move,
// nothing to count.
//
// Since C++17 this is GUARANTEED BY THE LANGUAGE for prvalues:
//
//     Widget make() { return Widget{...}; }
//     Widget w = make();              // exactly ONE construction. Always.
//
// For a NAMED local it's NRVO — done by every real compiler but not
// mandated (the fallback is a move, which is why moves must stay correct).
//
// Which brings us to the anti-pattern this exercise removes:
//
//     return std::move(result);      // PESSIMIZATION
//
// "I'll help the compiler move it out!" — but elision constructs in
// place, and std::move RUINS that: the return expression is no longer a
// plain prvalue/name, elision is off the table, and you've forced the
// very move you tried to save. Compilers even ship a warning for this
// pessimization (clang's is in -Wall) — helping the compiler here only
// gets in its way. (Exception that proves the rule: returning a MEMBER,
// like `return std::move(pair.first);` — that one's real, module 15.)
//
// Task: fix both factory functions so each construction is the ONLY
// event.
//   - all asserts pass: constructions == 1 and moves == 0, both times
// Constraints:
//   - make_named keeps its named local `w`; make_prvalue still builds
//     Widget{1}
//   - don't change Widget, main, or the asserts

#include <cassert>
#include <utility>

struct Widget {
    inline static int constructions = 0;
    inline static int moves = 0;

    int value;

    explicit Widget(int v) : value(v) { ++constructions; }
    Widget(Widget&& o) noexcept : value(o.value) { ++moves; }
    Widget(const Widget&) = delete;            // copies forbidden: moves
    Widget& operator=(const Widget&) = delete; // and elision only
    Widget& operator=(Widget&&) noexcept = default;
};

Widget make_prvalue() {
    return std::move(Widget{1});   // blocks GUARANTEED elision: +1 move
}

Widget make_named() {
    Widget w{2};
    return std::move(w);           // blocks NRVO: +1 move
}

int main() {
    Widget::constructions = 0;
    Widget::moves = 0;
    Widget a = make_prvalue();
    assert(a.value == 1);
    assert(Widget::constructions == 1);
    assert(Widget::moves == 0);        // guaranteed elision: zero, by law

    Widget::constructions = 0;
    Widget::moves = 0;
    Widget b = make_named();
    assert(b.value == 2);
    assert(Widget::constructions == 1);
    assert(Widget::moves == 0);        // NRVO: zero on any real compiler
    return 0;
}

// I AM NOT DONE
