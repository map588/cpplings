// extras2.cpp
//
// <filesystem> (C++17) ended the era of hand-rolled string surgery on
// paths. fs::path understands structure:
//
//     fs::path p = "/srv/pics/photo.raw";
//     p.filename()     →  "photo.raw"
//     p.stem()         →  "photo"
//     p.extension()    →  ".raw"
//     p.parent_path()  →  "/srv/pics"
//     dir / "file.txt"                      // operator/ joins, portably
//     p.replace_extension(".bak")           // mutates in place
//
// Three gotchas hide in the quiz below — settle each by reasoning
// (or by standing up a scratch program, a fine instinct to have):
//   - a filename with TWO dots: where does stem stop and extension
//     start? And does the extension carry its dot?
//   - a dotfile like ".gitignore": which part is stem, which is
//     extension?
//   - operator/ with an ABSOLUTE right-hand operand: a join, or
//     something more surprising?
//
// Task: implement rotated_name() — given a log path, produce its
// rotation sibling: same directory, same stem, ".1" inserted before
// the extension ("/var/log/app.log" → "/var/log/app.1.log"). Then
// settle the quiz TODOs.
//   - every assert passes
// Constraints:
//   - build rotated_name from the path's PARTS (decompose, edit,
//     reassemble) — no raw find/replace on the whole string
//   - it must work for any directory/stem/extension, not just the
//     two asserted paths
//   - quiz: change only the "TODO" strings

#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

fs::path rotated_name(const fs::path& log) {
    return log;   // TODO: take it apart, rebuild with ".1" in the seam
}

int main() {
    assert(rotated_name("/var/log/app.log") == "/var/log/app.1.log");
    assert(rotated_name("/tmp/x/trace.txt") == "/tmp/x/trace.1.txt");

    // The quiz:
    fs::path p = "/data/logs/app.2026.log";
    assert(p.extension() == "TODO");          // dot included? which dot?
    assert(p.stem() == "TODO");
    assert(p.parent_path() == "TODO");

    fs::path dotfile = ".gitignore";
    assert(dotfile.extension() == "TODO");    // trick question

    fs::path joined = fs::path("/usr") / "local" / "bin";
    assert(joined == "TODO");

    fs::path hijacked = fs::path("/usr") / "/etc";    // absolute rhs!
    assert(hijacked == "TODO");
    return 0;
}

// I AM NOT DONE
