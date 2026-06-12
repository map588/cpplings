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
// RedactingLogger below has TWO such near-misses:
//   - format() dropped the const
//   - prefix() takes std::string instead of const std::string&
//
// This is why `override` (C++11) exists. It's a CLAIM the compiler must
// verify: "this overrides something — error if not." Add it to both
// functions and the compiler will hand you the exact mismatches the
// runtime was hiding.
//
// Task: slap `override` on both derived functions, read the errors, fix
// the signatures. (Then adopt the habit: every override, everywhere,
// forever. clang's -Winconsistent-missing-override and
// -Wsuggest-override exist to enforce it across a codebase.)

// I AM NOT DONE

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
