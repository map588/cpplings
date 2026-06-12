// map1.cpp — solution

#include <cassert>
#include <map>
#include <string>

// const&: provably no insertion — operator[] wouldn't even compile here.
bool has_passed(const std::map<std::string, int>& scores,
                const std::string& name) {
    auto it = scores.find(name);             // one lookup, value included
    return it != scores.end() && it->second >= 60;
}

int main() {
    std::map<std::string, int> scores;
    ++scores["zoe"];
    scores["zoe"] += 89;
    scores["abe"] = 72;

    assert(scores.size() == 2);

    assert(has_passed(scores, "zoe"));
    assert(!has_passed(scores, "ghost"));
    assert(scores.size() == 2);              // no phantom insertions

    std::string order;
    for (const auto& [name, score] : scores) {
        order += name[0];
    }
    assert(order == "az");
    return 0;
}
