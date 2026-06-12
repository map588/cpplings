// errors3.cpp — solution

#include <cassert>
#include <string>

// No noexcept: this function's whole job involves a throwing callee,
// and garbage input is a legitimate runtime condition.
int parse_quantity(const std::string& text) {
    return std::stoi(text);
}

int main() {
    assert(parse_quantity("12") == 12);

    int qty = -1;
    try {
        qty = parse_quantity("twelve");
    } catch (const std::exception&) {
        qty = 0;                       // the catch works again
    }
    assert(qty == 0);

    static_assert(!noexcept(parse_quantity(std::string{})),
                  "parse_quantity may throw — it must not claim otherwise");
    return 0;
}
