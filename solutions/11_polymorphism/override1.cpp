// override1.cpp — solution

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
    // Signatures now match the base exactly — and `override` would
    // refuse to compile if they ever drift again.
    std::string prefix(const std::string& level) const override {
        return "[" + level + "|redacted] ";
    }
    std::string format(const std::string& msg) const override {
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
