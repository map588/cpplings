// errors5.cpp
//
// Exceptions cost asymmetrically: the happy path is essentially FREE
// (zero-cost tables, no branching), but an actual throw is expensive —
// unwinder machinery, type matching, thousands of cycles. That economy
// dictates usage: exceptions are for EXCEPTIONAL events, not for
// outcomes your program expects on every other call.
//
// next_token() below uses a throw to report "no more tokens" — a
// condition that happens once per parse BY DESIGN. That's not an error,
// it's a return value wearing a catch block. The cost is real, and so
// is the legibility damage: callers need try/catch to express an
// ordinary loop.
//
// Module 19 gave the right tool: std::optional — absence in the type,
// checked with an `if`, free.
//
// The decision table once more:
//   expected absence/outcome     → optional / expected(C++23)
//   exceptional, cross-layer     → exceptions (ctors especially — they
//                                  have no return channel!)
//   C / ABI boundaries           → error codes (+[[nodiscard]], C++17)
//
// Task: convert next_token to return std::optional<std::string>, and
// de-try/catch the loop in count_tokens.

#include <cassert>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <string_view>

class Tokenizer {
public:
    explicit Tokenizer(std::string_view text) : rest_(text) {}

    // Throws when input is exhausted. Every. Single. Parse.
    std::string next_token() {
        while (!rest_.empty() && rest_.front() == ' ') rest_.remove_prefix(1);
        if (rest_.empty()) {
            throw std::out_of_range("no more tokens");
        }
        auto space = rest_.find(' ');
        std::string token{rest_.substr(0, space)};
        rest_.remove_prefix(space == std::string_view::npos ? rest_.size()
                                                            : space);
        return token;
    }

private:
    std::string_view rest_;
};

int count_tokens(std::string_view text) {
    Tokenizer t(text);
    int n = 0;
    try {
        while (true) {
            t.next_token();
            ++n;
        }
    } catch (const std::out_of_range&) {
        // control flow by parachute
    }
    return n;
}

int main() {
    // The contract this exercise demands — absence in the return TYPE:
    Tokenizer probe("x");
    static_assert(std::is_same_v<decltype(probe.next_token()),
                                 std::optional<std::string>>,
                  "next_token should return optional, not throw");

    assert(count_tokens("the quick brown fox") == 4);
    assert(count_tokens("  padded   text ") == 2);
    assert(count_tokens("") == 0);
    return 0;
}

// I AM NOT DONE
