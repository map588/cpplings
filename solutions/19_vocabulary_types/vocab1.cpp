// vocab1.cpp — solution

#include <any>
#include <cassert>
#include <string>

int main() {
    std::any box;
    assert(box.has_value() == false);        // 1. empty until assigned

    box = std::string("payload");
    box = 42;
    assert(box.has_value() == true);         // 2. holds the int

    // 3. EXACT type only: it holds int, not long. No conversions here.
    bool long_cast_ok = (std::any_cast<long>(&box) != nullptr);
    assert(long_cast_ok == false);

    // 4. Value-form any_cast on the wrong type throws.
    bool threw = false;
    try {
        auto s = std::any_cast<std::string>(box);
        (void)s;
    } catch (const std::bad_any_cast&) {
        threw = true;
    }
    assert(threw == true);

    // 5.
    assert(std::any_cast<int>(box) == 42);
    return 0;
}
