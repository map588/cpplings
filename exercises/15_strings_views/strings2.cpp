// strings2.cpp
//
// std::string_view (C++17): a {const char*, length} pair. It VIEWS
// characters owned by someone else — no allocation, no copy, ever.
//
//   - substr() on a string COPIES the characters: a fresh allocation.
//     substr() on a string_view adjusts a pointer and a length: O(1).
//   - remove_prefix(n) / remove_suffix(n): shrink the view in place.
//   - A string_view PARAMETER accepts std::string, "literals", and
//     char arrays — one signature, zero copies for all three callers.
//
// extension() below is allocation-happy: const std::string& means the
// "config.tar.gz" literal materializes a std::string at every call, and
// .substr() allocates again for the return. For a parser that runs per
// line, that's the difference between O(1) and death by allocator.
//
// Task: make extension() allocation-free on every path — the final
// pointer-range assert is the proof, and today it fails.
//   - all asserts pass
//   - a true fix returns a view INTO the caller's characters:
//     ext.data() must land inside doc's own buffer
//   - the function's logic (last dot; empty result when there is none)
//     stays the same — only what flows in and out of it changes
//   - (returning a view is SAFE here: it points into the caller's
//     string, which outlives the call. strings3 shows when it isn't.)
// Constraints:
//   - don't change main() or the asserts
//   - no allocations anywhere in extension(): no std::string locals,
//     no copies of the characters

#include <cassert>
#include <string>
#include <string_view>

// Returns everything after the last '.', or "" if no dot.
std::string extension(const std::string& filename) {
    auto dot = filename.rfind('.');
    if (dot == std::string::npos) return "";
    return filename.substr(dot + 1);          // allocates a copy
}

int main() {
    assert(extension("photo.jpeg") == "jpeg");
    assert(extension("config.tar.gz") == "gz");     // LAST dot
    assert(extension("Makefile") == "");

    std::string doc = "report.pdf";
    assert(extension(doc) == "pdf");

    // The no-allocation proof — and today's failing assert: a true view
    // points INTO doc; the current substr() copy does not.
    std::string_view ext = extension(doc);
    assert(ext.data() >= doc.data() && ext.data() < doc.data() + doc.size());
    return 0;
}

// I AM NOT DONE
