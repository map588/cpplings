// strings4.cpp — solution

#include <cassert>
#include <charconv>
#include <string_view>

int parse_port(std::string_view text) {
    int value{};
    const char* end = text.data() + text.size();
    auto [ptr, ec] = std::from_chars(text.data(), end, value);

    if (ec != std::errc{}) return -1;        // no digits / overflow
    if (ptr != end) return -1;               // trailing junk
    if (value < 0 || value > 65535) return -1;   // signed parse: range-check
    return value;
}

int main() {
    assert(parse_port("8080") == 8080);
    assert(parse_port("0") == 0);
    assert(parse_port("65535") == 65535);

    assert(parse_port("https") == -1);
    assert(parse_port("") == -1);
    assert(parse_port("8080/tcp") == -1);
    assert(parse_port("99999") == -1);
    assert(parse_port("-1") == -1);          // -1 parses; range check rejects
    return 0;
}
