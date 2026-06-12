// move6.cpp — solution

#include <cassert>
#include <string>
#include <utility>
#include <vector>

std::vector<std::string> vault;

template <typename T>
void store(T&& value) {
    // forward: lvalues pass through as lvalues (copy), rvalues as
    // rvalues (move) — the caller's choice is preserved.
    vault.push_back(std::forward<T>(value));
}

int main() {
    std::string label = "a label far too long for the small-string "
                        "optimization, so theft is observable";
    std::string original = label;

    store(label);                    // copied; label intact
    assert(label == original);

    store(std::move(label));         // moved, as requested
    assert(vault.size() == 2);
    assert(vault[0] == original && vault[1] == original);
    return 0;
}
