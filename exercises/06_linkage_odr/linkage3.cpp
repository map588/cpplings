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
// linker, so every file can have its own. Two spellings:
//
//   static std::string normalize(...) { }   // the C way; functions only
//
//   namespace {                              // ANONYMOUS namespace —
//       std::string normalize(...) { }      // the C++ way. Works for
//       struct Helper { ... };               // types and templates too,
//   }                                        // which static can't do.
//
// Task: give each file's normalize() internal linkage (fix BOTH files —
// pick the anonymous-namespace spelling).

// I AM NOT DONE

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
