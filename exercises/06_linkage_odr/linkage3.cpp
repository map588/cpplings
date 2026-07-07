// linkage3.cpp        (compiled together with linkage3_render.cpp)
//
// Both files in this exercise wrote themselves a little private helper
// named normalize() — different bodies, same name, both at namespace
// scope. Each TU compiles fine; the LINKER refuses: duplicate symbol.
//
// And we got LUCKY it refused. If one definition had been inline (or a
// template, or a class method), this would be an ODR VIOLATION instead:
// "no diagnostic required" — the linker silently keeps ONE of the two
// bodies and both callers get it. Your function works in this build,
// breaks when the link order changes. The nastiest bug class in C++.
//
// A TU-private helper should have INTERNAL linkage — invisible to the
// linker, so every file can keep its own. C++ has two spellings for that:
// a keyword inherited from C that only works for functions and variables,
// and a namespace trick that also covers types and templates. Prefer the
// second.
//
// Task: make the two files link, run, and exit 0.
//   - both asserts pass — each file still runs ITS OWN normalize
// Constraints:
//   - fix BOTH files, and use the spelling that would also work for a type
//   - don't rename either normalize, don't change a body, don't merge them
//   - render_header must stay visible across TUs — only the helpers go
//     private

#include <cassert>
#include <string>

// This file's idea of "normalize": lowercase.
std::string normalize(std::string s) {
    for (char& c : s) {
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
    }
    return s;
}

std::string render_header(std::string title);   // from linkage3_render.cpp

int main() {
    assert(normalize("MiXeD") == "mixed");
    assert(render_header("intro") == "== INTRO ==");
    return 0;
}

// I AM NOT DONE
