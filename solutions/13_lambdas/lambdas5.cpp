// lambdas5.cpp — solution

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> words = {"fig", "watermelon", "kiwi", "banana"};

    // Generic lambda comparator: "does a belong before b?"
    std::sort(words.begin(), words.end(),
              [](const auto& a, const auto& b) { return a.size() > b.size(); });

    assert(words[0] == "watermelon");
    assert(words[1] == "banana");
    assert(words[2] == "kiwi");
    assert(words[3] == "fig");

    long big = std::count_if(words.begin(), words.end(),
                             [](const auto& w) { return w.size() > 4; });
    assert(big == 2);
    return 0;
}
