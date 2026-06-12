// valcat4.cpp — solution

#include <cassert>
#include <string>
#include <vector>

// Reference in, reference out: the result aliases the caller's element.
const std::string& longest_name(const std::vector<std::string>& names) {
    std::size_t best = 0;
    for (std::size_t i = 1; i < names.size(); ++i) {
        if (names[i].size() > names[best].size()) best = i;
    }
    return names[best];
}

int main() {
    std::vector<std::string> crew = {
        "bo",
        "alexandros-the-extraordinarily-long-named",
        "kit",
    };

    const std::string& winner = longest_name(crew);
    assert(winner == "alexandros-the-extraordinarily-long-named");
    assert(&winner == &crew[1]);
    return 0;
}
