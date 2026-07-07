// errors1.cpp
//
// Exception mechanics in one breath: `throw` an object; the runtime
// unwinds the stack until a matching `catch`; the handler's declared
// variable is initialized from the exception object.
//
// And that initialization is where today's bug lives. A catch clause
// declares a variable like any other, and the usual rules apply: if
// you declare a BASE-CLASS OBJECT (not a reference), the caught
// derived exception is COPIED INTO it — sliced (module 11!), derived
// parts gone, and the virtual what() reverts to the base's useless
// default.
//
// Rules to take away:
//   - a handler should see the ORIGINAL exception object, not a
//     base-typed copy of it — the same reasoning as passing
//     polymorphic types into functions (module 11 again)
//   - derive your exceptions from std::runtime_error (or kin): you
//     inherit a working what() and catch-ability as std::exception
//   - order catch clauses derived-first; the first MATCH wins, and
//     a std::exception clause swallows everything below it
//
// Task: make try_load report the real error message.
//   - the program runs and every assert passes
// Constraints:
//   - the handler must still catch ANY std::exception, not just
//     ConfigError
//   - do not change ConfigError, load_config, or main
//   - the fix lives entirely inside the catch clause's parentheses

#include <cassert>
#include <stdexcept>
#include <string>

class ConfigError : public std::runtime_error {
public:
    explicit ConfigError(const std::string& line)
        : std::runtime_error("bad config line: " + line) {}
};

void load_config(const std::string& line) {
    if (line.find('=') == std::string::npos) {
        throw ConfigError(line);
    }
}

std::string try_load(const std::string& line) {
    try {
        load_config(line);
        return "ok";
    } catch (std::exception e) {            // sliced!
        return e.what();
    }
}

int main() {
    assert(try_load("timeout=500") == "ok");
    assert(try_load("garbage") == "bad config line: garbage");
    return 0;
}

// I AM NOT DONE
