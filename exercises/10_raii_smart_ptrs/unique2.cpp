// unique2.cpp
//
// unique_ptr is MOVE-ONLY: its copy operations are =delete'd (module 08),
// because two unique_ptrs owning one object would double-delete (module
// 03). Ownership can therefore never change hands silently — giving an
// object away must be written out loud, at the call site, with the tool
// module 08 gave you for turning a copy into a transfer.
//
// After the transfer, the source unique_ptr is GUARANTEED null (one of the
// few moved-from states the standard fully specifies). Use-after-give-away
// becomes a checkable bug, not a heisenbug.
//
// The other half of the vocabulary: a function that only READS the object
// has no business in the ownership system at all. Demanding the ownership
// handle just to look at the data is rude — borrowers should accept the
// object itself, the way functions have borrowed things since module 02.
//
// Task: fix the two dishonest spots — archive()'s call site, and
// word_count()'s signature.
//   - all four asserts pass, including `doc == nullptr` after archiving
//   - word_count's parameter type no longer mentions a smart pointer
//   - archive() itself stays exactly as written; only its CALL changes
// Constraints:
//   - keep main's order of operations: count first, then archive
//   - the asserted VALUES stay as they are (you may adapt how the calls
//     inside the asserts are spelled)
//   - don't copy the string to sidestep the ownership question

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

// I AM NOT DONE
