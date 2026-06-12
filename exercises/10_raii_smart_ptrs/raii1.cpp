// raii1.cpp
//
// The manual acquire/release discipline:
//
//     int h = acquire_handle();
//     ...                          // ← every path through here
//     release_handle(h);           //   must reach this line
//
// It survives until the first early return. Or the first exception. Or
// the first `continue` someone adds in review. process() below has
// exactly that bug.
//
// RAII is the fix: wrap the resource in an object whose DESTRUCTOR
// releases it. Module 07 taught that destructors run at scope exit on
// every path — so the release becomes impossible to skip. That's the
// whole idiom. string, vector, unique_ptr, lock_guard, fstream: all the
// same shape.
//
// Task: finish the HandleGuard class and use it in process(). Delete the
// manual release calls. Guard objects own their resource alone — copying
// one would mean double-release, so copying is already deleted for you
// (module 08's =delete).

// I AM NOT DONE

#include <cassert>

// --- a fake device API (pretend it's some C library) ------------------
inline int open_handles = 0;
int acquire_handle()        { ++open_handles; return 7; }
void release_handle(int)    { --open_handles; }
int read_sample(int, int i) { return i == 3 ? -1 : i * 10; }
// ----------------------------------------------------------------------

class HandleGuard {
public:
    HandleGuard() : h_(acquire_handle()) {}

    // TODO: the entire point of this class.

    HandleGuard(const HandleGuard&) = delete;
    HandleGuard& operator=(const HandleGuard&) = delete;

    int get() const { return h_; }

private:
    int h_;
};

// Sums the first n samples; gives up early on a negative one.
int process(int n) {
    int h = acquire_handle();
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        int s = read_sample(h, i);
        if (s < 0) {
            return -1;            // ← the handle leaks right here
        }
        sum += s;
    }
    release_handle(h);
    return sum;
}

int main() {
    assert(process(3) == 30);
    assert(open_handles == 0);    // the happy path cleans up...

    assert(process(5) == -1);     // ...this one hits the bad sample at i==3,
    assert(open_handles == 0);    // takes the early return, and leaks.
    return 0;
}
