// optional2.cpp — solution

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
    return lookup("retries").value_or(3);   // absence handled, in the type
}

int main() {
    assert(lookup("port").value() == 8080);

    assert(retry_limit() == 3);

    bool threw = false;
    try {
        lookup("retries").value();
    } catch (const std::bad_optional_access&) {
        threw = true;
    }
    assert(threw == true);    // value() on empty: throws. That's its job.
    return 0;
}
