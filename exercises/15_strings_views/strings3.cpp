// strings3.cpp
//
// The bill for string_view's free lunch: it keeps NOTHING alive.
//
// strings2's return-a-view was safe because the view pointed into the
// CALLER'S string. Here the caller passes a TEMPORARY:
//
//     std::string_view name = first_word(make_header());
//                          // ^ the std::string dies at this semicolon
//     name[0];             // dangling — reads freed heap memory
//
// first_word() itself is blameless; no local rule was broken. The
// SUM of "function returns a view into its argument" + "argument was a
// temporary" is the bug. That's what makes view lifetimes hard: the
// error is in the composition, not in any one line. (clang's
// -Wdangling-gsl flags the simplest forms; ASan catches the rest, as
// you're about to see.)
//
// Rules of thumb:
//   - string_view parameters: fine (argument outlives the call)
//   - returning a view INTO a parameter: caller must own the argument
//   - storing a string_view (member, global): you now have a manual
//     lifetime contract. Documented owner or std::string member.
//
// Task: fix main() so the asserts pass and ASan stays quiet.
//   - `name` remains a std::string_view produced by first_word()
//   - the characters it views must be owned by something still alive
//     when the asserts run
// Constraints:
//   - make_header() and first_word() are blameless — don't change them
//   - don't turn `name` into a std::string, and don't paste the
//     expected text in as a literal
//   - the fix lives in main() and is small

#include <cassert>
#include <string>
#include <string_view>

// Builds the header line (long enough to live on the heap, so the
// sanitizer can see the crime).
std::string make_header() {
    return "Content-Type-With-Extremely-Verbose-Name: application/json";
}

// Returns a view of everything before the first ':'.
std::string_view first_word(std::string_view line) {
    return line.substr(0, line.find(':'));
}

int main() {
    std::string_view name = first_word(make_header());
    // The temporary string died on the line above. `name` is a treasure
    // map to a demolished building:
    assert(name == "Content-Type-With-Extremely-Verbose-Name");
    assert(name.size() == 40);
    return 0;
}

// I AM NOT DONE
