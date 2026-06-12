// extras3.cpp — solution

#include <cassert>
#include <format>
#include <string>

std::string describe_transfer(const std::string& from, const std::string& to,
                              double mb, int percent) {
    return std::format("{} -> {}: {:>8.2f} MB ({}%)", from, to, mb, percent);
}

int main() {
    assert(describe_transfer("alice", "bob", 42.5, 7)
           == "alice -> bob:    42.50 MB (7%)");
    assert(describe_transfer("db", "tape", 1337.126, 99)
           == "db -> tape:  1337.13 MB (99%)");

    assert(std::format("{:>6}", 42) == "    42");
    assert(std::format("{:#06x}", 42) == "0x002a");
    assert(std::format("{0}{1}{0}", "|", "core") == "|core|");
    return 0;
}
