// extras2.cpp
//
// <filesystem> (C++17) ended the era of hand-rolled string surgery on
// paths. fs::path understands structure:
//
//     fs::path p = "/data/logs/app.2026.log";
//     p.filename()     →  "app.2026.log"
//     p.stem()         →  "app.2026"        (filename minus LAST ext)
//     p.extension()    →  ".log"            (the LAST dot, with the dot!)
//     p.parent_path()  →  "/data/logs"
//     dir / "file.txt"                       // operator/ joins, portably
//     p.replace_extension(".bak")           // mutates in place
//
// Gotchas the asserts below cover: extension() INCLUDES the dot (and
// only the last one — compare module 15's extension(), which stripped
// it); operator/ with an ABSOLUTE right-hand side REPLACES the left
// entirely; dotfiles like ".gitignore" are all stem, no extension.
//
// Task: implement rotated_name() — given a log path, produce its
// rotation sibling: same directory, same stem, ".1" inserted before
// the extension ("/var/log/app.log" → "/var/log/app.1.log"). Then
// settle the quiz TODOs.

#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

fs::path rotated_name(const fs::path& log) {
    return log;   // TODO: parent / (stem + ".1" + extension)
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
