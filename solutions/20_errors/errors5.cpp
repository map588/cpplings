// errors5.cpp — solution

#include <cassert>
#include <optional>
#include <string>
#include <type_traits>
#include <string_view>

class Tokenizer {
public:
    explicit Tokenizer(std::string_view text) : rest_(text) {}

    // Exhaustion is an expected outcome → it lives in the return type.
    std::optional<std::string> next_token() {
        while (!rest_.empty() && rest_.front() == ' ') rest_.remove_prefix(1);
        if (rest_.empty()) {
            return std::nullopt;
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
    while (t.next_token()) {       // an ordinary loop again
        ++n;
    }
    return n;
}

int main() {
    Tokenizer probe("x");
    static_assert(std::is_same_v<decltype(probe.next_token()),
                                 std::optional<std::string>>,
                  "next_token should return optional, not throw");
    (void)probe;

    assert(count_tokens("the quick brown fox") == 4);
    assert(count_tokens("  padded   text ") == 2);
    assert(count_tokens("") == 0);
    return 0;
}
