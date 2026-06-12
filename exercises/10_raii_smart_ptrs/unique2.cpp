// unique2.cpp
//
// unique_ptr is MOVE-ONLY: its copy operations are =delete'd (module 08),
// because two unique_ptrs owning one object would double-delete (module
// 03). That makes ownership transfer EXPLICIT and visible at every call
// site:
//
//     ship(std::move(cargo));        // "I'm giving this away" — in code
//
// After the move, the source unique_ptr is GUARANTEED null (one of the
// few moved-from states the standard fully specifies). Use-after-give-away
// becomes a checkable bug, not a heisenbug.
//
// API vocabulary this creates:
//
//     void take(std::unique_ptr<T> p);   // "I consume/own this now"
//     void use(const T& t);  /  T* t;    // "just lending it to me"
//
// A function that only reads the object should take a reference (or raw
// pointer if null is meaningful) — NOT a smart pointer. Borrowers don't
// participate in ownership.
//
// Task: two fixes. archive() takes ownership — say so at the call site.
// word_count() merely reads — fix its signature to borrow.

// I AM NOT DONE

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

std::vector<std::unique_ptr<std::string>> archived;

void archive(std::unique_ptr<std::string> doc) {
    archived.push_back(std::move(doc));
}

// Only READS the document — so why would it demand the ownership handle?
std::size_t word_count(std::unique_ptr<std::string> doc) {
    std::size_t words = 0;
    bool in_word = false;
    for (char c : *doc) {
        bool space = (c == ' ');
        if (!space && !in_word) ++words;
        in_word = !space;
    }
    return words;
}

int main() {
    auto doc = std::make_unique<std::string>("the quick brown fox");

    assert(word_count(doc) == 4);     // borrowing shouldn't require a move

    archive(doc);                     // ownership transfer: must be explicit
    assert(doc == nullptr);           // moved-from unique_ptr: null, always
    assert(archived.size() == 1);
    assert(*archived[0] == "the quick brown fox");
    return 0;
}
