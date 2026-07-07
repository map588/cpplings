// fd1.cpp
//
// (POSIX exercise: open/read/close come from <fcntl.h> and <unistd.h>,
// and /proc is Linux. Not portable C++ — very portable career knowledge.)
//
// The kernel hands you resources as file descriptors: plain ints.
// Every process has a hard cap on open fds (`ulimit -n` — often 1024).
// Leak one per request and a long-running daemon dies days later with
// EMFILE ("too many open files") — the kind of bug a five-second test
// run never shows. RAII is the cure, exactly as in raii1. But notice
// that unique_ptr (C++11) can't be the guard this time. deleter1
// adapted it to any POINTER-shaped C API; an fd is not pointer-shaped:
//
//   - the empty state is -1, not nullptr (0 is a VALID fd — stdin!)
//   - there is nothing to `delete`; close() takes the int itself
//
// For int-shaped handles you write the small guard by hand: store the
// fd, close it in the destructor — and since two owners would mean two
// close() calls, make it move-only, module 08/09 style: copying
// deleted, moving steals the fd and leaves -1 behind.
//
// The leak below is MEASURABLE: main counts this process's open fds by
// listing /proc/self/fd (std::filesystem, C++17) before and after the
// reload loop.
//
// Task: finish FdGuard, convert reload_config to use it, and make every
// assert pass.
//   - after 8 reloads the fd count must equal the baseline
//   - moving a guard must empty the source (-1) and close the fd
//     exactly once
// Constraints:
//   - don't change count_open_fds or any assert line
//   - reload_config keeps its early "unchanged → skip" return
//   - when you're done, the only ::close in the file lives inside
//     FdGuard (patching each exit path by hand is what raii1 buried)

#include <cassert>
#include <fcntl.h>      // POSIX: open, O_RDONLY
#include <filesystem>
#include <unistd.h>     // POSIX: read, close
#include <utility>

constexpr const char* kConfigPath = "/proc/self/status";  // always exists

// How many fds does this process have open right now? (The directory
// iterator itself holds one while we count — same offset both times,
// so comparisons stay honest.)
int count_open_fds() {
    int n = 0;
    for (const auto& entry :
         std::filesystem::directory_iterator("/proc/self/fd")) {
        (void)entry;    // only the count matters
        ++n;
    }
    return n;
}

// An fd guard. (Production versions of this exact class: Android's
// unique_fd, Chromium's ScopedFD. Everyone writes it once.)
class FdGuard {
public:
    explicit FdGuard(int fd) noexcept : fd_(fd) {}

    // TODO: the reason this class exists.

    // Move transfers ownership. As shipped, BOTH objects believe they
    // own the fd afterwards — which destructor closes it?
    FdGuard(FdGuard&& other) noexcept : fd_(other.fd_) {}
    FdGuard& operator=(FdGuard&& other) noexcept {
        fd_ = other.fd_;    // and what happens to the fd we already held?
        return *this;
    }

    FdGuard(const FdGuard&) = delete;             // two owners = two closes
    FdGuard& operator=(const FdGuard&) = delete;

    int get() const noexcept { return fd_; }

private:
    int fd_ = -1;   // -1 means "empty": moved-from or never opened
};

// Re-reads the config file; returns false only on I/O failure. Skips
// the (pretend) expensive re-parse when nothing changed this generation.
bool reload_config(int generation) {
    int fd = ::open(kConfigPath, O_RDONLY);
    if (fd < 0) {
        return false;
    }

    char buf[256];
    if (::read(fd, buf, sizeof buf) <= 0) {
        return false;                  // an exit path...
    }

    if (generation % 2 == 0) {
        return true;                   // "unchanged → skip" — and another
    }

    // ... pretend to re-parse buf into the live config here ...
    ::close(fd);
    return true;
}

int main() {
    const int baseline = count_open_fds();

    // A daemon's life, compressed: 8 config reloads.
    for (int gen = 0; gen < 8; ++gen) {
        assert(reload_config(gen));
    }
    assert(count_open_fds() == baseline);   // no fd may outlive its reload

    // The guard's ownership contract, measured the same way:
    {
        FdGuard a(::open(kConfigPath, O_RDONLY));
        assert(a.get() >= 0);

        FdGuard b(std::move(a));
        assert(a.get() == -1);                     // emptied, not copied
        assert(b.get() >= 0);
        assert(count_open_fds() == baseline + 1);  // one fd, one owner
    }
    assert(count_open_fds() == baseline);          // closed exactly once
    return 0;
}

// I AM NOT DONE
