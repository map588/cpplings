// unique1.cpp
//
// raii1's HandleGuard already exists in the standard library, generalized:
// std::unique_ptr<T> (C++11) owns a heap object and deletes it in its
// destructor. Sole owner, every exit path, zero overhead:
//
//     static_assert(sizeof(std::unique_ptr<Widget>) == sizeof(Widget*));
//
// Same size as the raw pointer it replaces (with the default deleter) —
// ownership costs NOTHING at runtime. There is no excuse for an owning
// raw pointer in modern C++.
//
// Create with std::make_unique<T>(args...) (C++14):
//
//     auto w = std::make_unique<Widget>(42);
//     w->method();   *w;   w.get()           // use like a pointer
//
// (Prefer make_unique over unique_ptr<T>(new T): no naked new to lose
// track of, and historically it also fixed an exception-safety hole in
// multi-argument calls.)
//
// Task: build_report leaks on the early-return path — the same disease
// raii1 had, in heap-allocation form. Convert it to make_unique and
// delete every `delete`.

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
    Report* r = new Report(title);
    r->lines = lines;

    if (lines == 0) {
        return "(empty)";                   // leak: nobody deletes r
    }

    std::string out = r->title + ": " + std::to_string(r->lines) + " lines";
    delete r;
    return out;
}

int main() {
    assert(build_report("daily", 12) == "daily: 12 lines");
    assert(Report::alive == 0);

    assert(build_report("weekly", 0) == "(empty)");
    assert(Report::alive == 0);             // the early return leaked one
    return 0;
}

// I AM NOT DONE
