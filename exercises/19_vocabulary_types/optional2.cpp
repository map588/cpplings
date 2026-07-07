// optional2.cpp
//
// Two ways to reach into an optional, two different failure modes:
//
//   *opt / opt->member    NO CHECK. Empty optional → UNDEFINED BEHAVIOR
//                         (yes, really — it's the unchecked-fast path,
//                         pointer-dereference rules). Use after you've
//                         established presence.
//
//   opt.value()           CHECKED. Empty optional → throws
//                         std::bad_optional_access. An uncaught throw
//                         terminates — loudly, with a stack trace. When
//                         in doubt, this is the honest accessor.
//
// (And value_or(x) never fails at all. C++23 adds monadic chains:
// opt.and_then(f).transform(g) — reading note.)
//
// The config below is missing "retries". The shipped code dereferences
// the empty optional anyway — UB that happens to "work", until lookup()
// is reordered or the optimizer gets clever.
//
// Task: make the absence handling explicit.
//   - retry_limit() returns the documented default (3) when the key is
//     missing, and the configured value when it's present
//   - no UB, no throwing on the missing-key path
//   - the quiz assert at the bottom: replace the TODO answer with the
//     truth
// Constraints:
//   - do not add "retries" to config; do not change lookup() or the
//     other asserts in main
//   - no try/catch in retry_limit — pick the accessor whose failure
//     mode you can rule out (the table above is the whole decision)

#include <cassert>
#include <map>
#include <optional>
#include <string>

std::map<std::string, int> config = {{"timeout_ms", 500}, {"port", 8080}};

std::optional<int> lookup(const std::string& key) {
    auto it = config.find(key);
    if (it == config.end()) return std::nullopt;
    return it->second;
}

int retry_limit() {
    std::optional<int> r = lookup("retries");
    return *r;          // empty → UB. The worst kind of "works on my machine".
}

int main() {
    assert(lookup("port").value() == 8080);     // present: value() is fine

    assert(retry_limit() == 3);                 // absent: default, please

    // ...and prove you know what value() does on empty — predict:
    bool threw = false;
    try {
        lookup("retries").value();
    } catch (const std::bad_optional_access&) {
        threw = true;
    }
    assert(threw == false);   // TODO: true or false?
    return 0;
}

// I AM NOT DONE
