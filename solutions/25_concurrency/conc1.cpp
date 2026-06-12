// conc1.cpp — solution

#include <cassert>
#include <string>
#include <thread>
#include <vector>

int count_words(const std::string& text) {
    int n = 0;
    bool in_word = false;
    for (char c : text) {
        bool space = (c == ' ');
        if (!space && !in_word) ++n;
        in_word = !space;
    }
    return n;
}

int word_total(const std::vector<std::string>& docs) {
    std::vector<int> per_doc(docs.size());

    {
        // jthread (C++20): joins on destruction — leaving this scope IS
        // the synchronization point.
        std::vector<std::jthread> workers;
        for (std::size_t i = 0; i < docs.size(); ++i) {
            workers.emplace_back([&docs, &per_doc, i] {
                per_doc[i] = count_words(docs[i]);
            });
        }
    }   // all joined here

    int total = 0;
    for (int n : per_doc) total += n;
    return total;
}

int main() {
    std::vector<std::string> docs = {
        "the quick brown fox",
        "jumps over",
        "the lazy dog",
    };
    assert(word_total(docs) == 9);
    return 0;
}
