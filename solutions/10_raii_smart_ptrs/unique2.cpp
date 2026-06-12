// unique2.cpp — solution

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

std::vector<std::unique_ptr<std::string>> archived;

void archive(std::unique_ptr<std::string> doc) {
    archived.push_back(std::move(doc));
}

// Borrower: takes a reference, stays out of the ownership system.
std::size_t word_count(const std::string& doc) {
    std::size_t words = 0;
    bool in_word = false;
    for (char c : doc) {
        bool space = (c == ' ');
        if (!space && !in_word) ++words;
        in_word = !space;
    }
    return words;
}

int main() {
    auto doc = std::make_unique<std::string>("the quick brown fox");

    assert(word_count(*doc) == 4);            // lend the object itself

    archive(std::move(doc));                  // give it away, visibly
    assert(doc == nullptr);
    assert(archived.size() == 1);
    assert(*archived[0] == "the quick brown fox");
    return 0;
}
