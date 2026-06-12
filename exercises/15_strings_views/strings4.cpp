// strings4.cpp
//
// String → number, three generations:
//
//   atoi("42")           // C: returns 0 on error. 0 is also a number.
//                        // Good luck.
//   std::stoi(s)         // C++11: throws on error, allocates a
//                        // std::string, consults the global locale.
//   std::from_chars      // C++17: no exceptions, no allocation, no
//                        // locale. The fast, composable one.
//
// from_chars's interface is C-flavored but honest:
//
//     int value{};
//     auto [ptr, ec] = std::from_chars(first, last, value);
//     // ec == std::errc{}        → success
//     // ptr → first unparsed char (great for parsing "123x45" stepwise)
//     // on failure, `value` is untouched
//
// (That auto [ptr, ec] unpacking is a structured binding, C++17 —
// module 19 covers it properly.)
//
// Task: implement parse_port. Contract per the asserts: digits parse;
// garbage and out-of-range values return -1; trailing junk after the
// number is also a failure (use the returned ptr!).

// I AM NOT DONE

#include <cassert>
#include <charconv>
#include <string_view>

// Returns the port number, or -1 if `text` isn't exactly a number
// in [0, 65535].
int parse_port(std::string_view text) {
    return -1;   // TODO: std::from_chars(text.data(), text.data() + text.size(), ...)
}

int main() {
    assert(parse_port("8080") == 8080);
    assert(parse_port("0") == 0);
    assert(parse_port("65535") == 65535);

    assert(parse_port("https") == -1);       // not a number
    assert(parse_port("") == -1);
    assert(parse_port("8080/tcp") == -1);    // trailing junk
    assert(parse_port("99999") == -1);       // out of range
    assert(parse_port("-1") == -1);          // parses (int is signed!) but
                                             // must fail the range check
    return 0;
}
