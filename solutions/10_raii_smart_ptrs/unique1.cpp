// unique1.cpp — solution

#include <cassert>
#include <memory>
#include <string>

struct Report {
    inline static int alive = 0;

    std::string title;
    int lines = 0;

    explicit Report(std::string t) : title(std::move(t)) { ++alive; }
    ~Report() { --alive; }
};

std::string build_report(const std::string& title, int lines) {
    auto r = std::make_unique<Report>(title);
    r->lines = lines;

    if (lines == 0) {
        return "(empty)";                   // unique_ptr cleans up here
    }

    return r->title + ": " + std::to_string(r->lines) + " lines";
}                                           // ...and here

int main() {
    assert(build_report("daily", 12) == "daily: 12 lines");
    assert(Report::alive == 0);

    assert(build_report("weekly", 0) == "(empty)");
    assert(Report::alive == 0);

    // Ownership, free of charge:
    static_assert(sizeof(std::unique_ptr<Report>) == sizeof(Report*));
    return 0;
}
