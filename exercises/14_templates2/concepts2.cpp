// concepts2.cpp
//
// concepts1 composed ready-made concepts. To describe an INTERFACE —
// "has these members, returning these types" — you write a REQUIRES
// EXPRESSION:
//
//     template <typename T>
//     concept Drawable = requires(T t, int n) {
//     //                 ^ fake variables to write requirements with
//         t.resize(n);                  // simple: must compile
//         typename T::id_type;          // type: member type must exist
//         { t.draw() } -> std::convertible_to<std::string>;
//         // compound: must compile AND the result must satisfy
//         // the concept on the right
//         { t.visible() } noexcept -> std::same_as<bool>;
//         // can demand noexcept too
//     };
//
// Nothing runs — the requirements are checked against declarations
// only. This is "duck typing, statically checked": any type with the
// right shape satisfies the concept, no inheritance involved (compare
// module 11's virtual interfaces!).
//
// Task: replace Renderable's `= true` stub with a requires expression
// that makes all four static_asserts hold.
//   - the four test types below ARE the spec: Sprite and Backdrop must
//     satisfy the concept; Sound and Glitch must not
//   - Backdrop's render() returns const char* and must still pass —
//     so "returns exactly std::string" is too tight a requirement
//   - Glitch's layer() returns double and must fail — so "converts
//     to int" is too loose a requirement
// Constraints:
//   - don't touch the test types or the static_asserts
//   - two requirements are enough; every extra one you add risks
//     rejecting a type that must pass

#include <concepts>
#include <string>

template <typename T>
concept Renderable = true;   // TODO: a requires expression

// --- types to test against --------------------------------------------
struct Sprite {
    std::string render() const { return "sprite"; }
    int layer() const { return 1; }
};

struct Backdrop {
    const char* render() const { return "backdrop"; }   // converts: fine
    int layer() const { return 0; }
};

struct Sound {
    int volume() const { return 11; }    // no render, no layer
};

struct Glitch {
    std::string render() const { return "?"; }
    double layer() const { return 0.5; }   // layer must be EXACTLY int
};
// ----------------------------------------------------------------------

static_assert(Renderable<Sprite>);
static_assert(Renderable<Backdrop>);
static_assert(!Renderable<Sound>);
static_assert(!Renderable<Glitch>);

int main() { return 0; }

// I AM NOT DONE
