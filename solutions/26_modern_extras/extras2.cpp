// extras2.cpp — solution

#include <cassert>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

fs::path rotated_name(const fs::path& log) {
    return log.parent_path()
           / (log.stem().string() + ".1" + log.extension().string());
}

int main() {
    assert(rotated_name("/var/log/app.log") == "/var/log/app.1.log");
    assert(rotated_name("/tmp/x/trace.txt") == "/tmp/x/trace.1.txt");

    fs::path p = "/data/logs/app.2026.log";
    assert(p.extension() == ".log");          // last dot only, dot included
    assert(p.stem() == "app.2026");           // everything before it
    assert(p.parent_path() == "/data/logs");

    fs::path dotfile = ".gitignore";
    assert(dotfile.extension() == "");        // dotfiles are ALL stem

    fs::path joined = fs::path("/usr") / "local" / "bin";
    assert(joined == "/usr/local/bin");

    fs::path hijacked = fs::path("/usr") / "/etc";
    assert(hijacked == "/etc");               // absolute rhs replaces lhs!
    return 0;
}
