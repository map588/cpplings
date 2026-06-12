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
// (And value_or(x) never fails at all — the right call when a fallback
// exists. C++23 adds monadic chains: opt.and_then(f).transform(g) —
// reading note.)
//
// The config below is missing "retries". The shipped code dereferences
// the empty optional anyway — UB that happens to "work", until lookup()
// is reordered or the optimizer gets clever. Make the absence handling
// explicit instead.
//
// Task: fix retry_limit() to use the documented default (3) when the
// key is missing — without UB and without throwing.

// I AM NOT DONE

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
