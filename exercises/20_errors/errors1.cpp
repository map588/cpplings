// errors1.cpp
//
// Exception mechanics in one breath: `throw` an object; the runtime
// unwinds the stack until a matching `catch`; the exception object
// travels by value to the handler.
//
// And the handler is where today's bug lives:
//
//     catch (std::exception e)          // BY VALUE: the ParseError is
//                                       // COPIED INTO a std::exception —
//                                       // sliced (module 11!). e.what()
//                                       // is now the base class's
//                                       // useless default.
//     catch (const std::exception& e)   // by const ref: the original
//                                       // object, virtual what() intact
//
// Rules that follow from slicing + the type system:
//   - throw by value, catch by const reference. Always.
//   - derive your exceptions from std::runtime_error (or kin): you
//     inherit a working what() and catch-ability as std::exception
//   - order catch clauses derived-first; the first MATCH wins, and
//     a std::exception clause swallows everything below it
//
// Task: fix the catch clause (and see the real message reappear).

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
