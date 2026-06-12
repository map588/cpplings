// strings2.cpp — solution

#include <cassert>
#include <string>
#include <string_view>

// View in, view out: zero allocations on any path.
std::string_view extension(std::string_view filename) {
    auto dot = filename.rfind('.');
    if (dot == std::string_view::npos) return "";
    return filename.substr(dot + 1);          // O(1): pointer + length math
}

int main() {
    assert(extension("photo.jpeg") == "jpeg");
    assert(extension("config.tar.gz") == "gz");
    assert(extension("Makefile") == "");

    std::string doc = "report.pdf";
    assert(extension(doc) == "pdf");

    std::string_view ext = extension(doc);
    assert(ext.data() >= doc.data() && ext.data() < doc.data() + doc.size());
    return 0;
}
