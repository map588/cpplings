// errors1.cpp — solution

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
    } catch (const std::exception& e) {      // the real object, unsliced
        return e.what();
    }
}

int main() {
    assert(try_load("timeout=500") == "ok");
    assert(try_load("garbage") == "bad config line: garbage");
    return 0;
}
