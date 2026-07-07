// override1.cpp
//
// The silent failure mode of virtual functions: HIDING instead of
// overriding. To override, the derived signature must match the base
// EXACTLY — name, parameters, const-ness, ref-qualifiers. Get one detail
// wrong and you've declared a brand-new, unrelated function that merely
// shadows the base one. No error. No warning (by default). Calls through
// the base run the base version; your "override" only runs when someone
// calls it directly on the derived type.
//
// RedactingLogger below has TWO such near-misses — a different detail is
// wrong in each function.
//
// This is why `override` (C++11) exists. It's a CLAIM the compiler must
// verify: "this overrides something — error if not." Make the claim and
// the compiler hands you the exact mismatches the runtime was hiding.
// (Adopt the habit: every override, everywhere, forever. clang's
// -Winconsistent-missing-override and -Wsuggest-override exist to
// enforce it across a codebase.)
//
// Task: make RedactingLogger genuinely override both functions.
//   - the assert passes: a call through Logger& reaches the redacting
//     versions of BOTH functions
//   - both derived functions carry the compiler-verified claim, so any
//     future signature drift fails to compile instead of hiding
// Constraints:
//   - Logger (the base) is correct as written — don't modify it
//   - keep the redacted output text exactly as it is
//   - don't dodge dispatch by calling the derived type directly

#include <cassert>
#include <string>

struct Logger {
    virtual std::string prefix(const std::string& level) const {
        return "[" + level + "] ";
    }
    virtual std::string format(const std::string& msg) const {
        return prefix("info") + msg;
    }
    virtual ~Logger() = default;
};

struct RedactingLogger : Logger {
    // Intended to override... does it?
    std::string prefix(std::string level) const {
        return "[" + level + "|redacted] ";
    }
    std::string format(const std::string& msg) {
        return prefix("info") + "***" + msg + "***";
    }
};

std::string log_via_base(const Logger& l, const std::string& msg) {
    return l.format(msg);
}

int main() {
    RedactingLogger redacting;
    assert(log_via_base(redacting, "pw=hunter2")
           == "[info|redacted] ***pw=hunter2***");
    return 0;
}

// I AM NOT DONE
