// aggregate1.cpp
//
// A struct with no user-declared constructors (nor private members,
// virtuals, base-class exotica) is an AGGREGATE: braces initialize the
// members directly, in declaration order, no constructor involved.
//
// Positional aggregate init is fragile, though. Config below used to be
// {port, verbose, retries} — call sites wrote Config{9000, true}. Then
// someone added `host` as the FIRST member, and every positional call
// site shifted meaning by one. Here you got lucky: int doesn't convert
// to string_view, so it's a loud compile error. Had the new member been
// another int, the code would have compiled and meant something else.
//
// C++20's DESIGNATED INITIALIZERS fix the fragility (C had them since
// C99!):
//
//     struct Point { int x = 0; int y = 0; int z = 0; };
//     Point p{.x = 1, .z = 5};      // y keeps its default
//
// Named, self-documenting, immune to member insertions ABOVE — and
// unmentioned members keep their NSDMI defaults. Rules: designators must
// follow declaration order (gcc enforces; clang downgrades reordering to
// a warning — don't rely on it), and you can't mix positional with
// designated.
//
// One interaction to remember: declaring ANY constructor — even
// `Config() = default;` — un-aggregates the type in C++20, and
// designated init stops working. The static_assert guards that.
//
// Task: rewrite both initializers so member NAMES, not positions, do
// the talking.
//   - compiles (static_assert included) and every assert passes
//   - `debug` states ONLY what differs from the defaults
// Constraints:
//   - don't reorder or edit Config's members; don't add constructors
//   - don't change the asserts

#include <cassert>
#include <string_view>
#include <type_traits>

struct Config {
    std::string_view host = "localhost";   // the member that broke everyone
    int port = 8080;
    bool verbose = false;
    int retries = 3;
};

static_assert(std::is_aggregate_v<Config>,
              "keep Config constructor-free: designated init depends on it");

int main() {
    // Written when port was the first member. It isn't anymore.
    Config server{9000, true};
    assert(server.host == "localhost");
    assert(server.port == 9000);
    assert(server.verbose == true);
    assert(server.retries == 3);             // untouched → NSDMI default

    // Wants only verbose=true; repeats two defaults positionally to get
    // there. Say only what differs:
    Config debug{"localhost", 8080, true};
    assert(debug.verbose == true);
    assert(debug.port == 8080);
    return 0;
}

// I AM NOT DONE
