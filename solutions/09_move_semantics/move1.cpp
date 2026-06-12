// move1.cpp — solution

#include <cassert>
#include <string>
#include <utility>

struct Archive {
    std::string contents;

    void store(std::string text) {
        contents = std::move(text);    // hand the buffer onward
    }
};

int main() {
    std::string entry = "a string long enough to defeat the small-string "
                        "optimization and force a heap allocation";
    const char* heap_buffer = entry.data();

    Archive log;
    log.store(std::move(entry));

    assert(log.contents.data() == heap_buffer);   // the very same buffer

    entry = "reused";
    assert(entry == "reused");
    return 0;
}
