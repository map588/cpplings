// fd1.cpp — solution

#include <cassert>
#include <fcntl.h>      // POSIX: open, O_RDONLY
#include <filesystem>
#include <unistd.h>     // POSIX: read, close
#include <utility>

constexpr const char* kConfigPath = "/proc/self/status";  // always exists

int count_open_fds() {
    int n = 0;
    for (const auto& entry :
         std::filesystem::directory_iterator("/proc/self/fd")) {
        (void)entry;    // only the count matters
        ++n;
    }
    return n;
}

class FdGuard {
public:
    explicit FdGuard(int fd) noexcept : fd_(fd) {}

    ~FdGuard() {
        if (fd_ >= 0) ::close(fd_);   // every path, every time — and never
    }                                 // on a moved-from (-1) guard

    // steal-and-mark: std::exchange (C++14) does both in one expression
    FdGuard(FdGuard&& other) noexcept : fd_(std::exchange(other.fd_, -1)) {}
    FdGuard& operator=(FdGuard&& other) noexcept {
        if (this != &other) {
            if (fd_ >= 0) ::close(fd_);          // release what we held
            fd_ = std::exchange(other.fd_, -1);  // then take theirs
        }
        return *this;
    }

    FdGuard(const FdGuard&) = delete;             // two owners = two closes
    FdGuard& operator=(const FdGuard&) = delete;

    int get() const noexcept { return fd_; }

private:
    int fd_ = -1;
};

bool reload_config(int generation) {
    FdGuard fd(::open(kConfigPath, O_RDONLY));
    if (fd.get() < 0) {
        return false;
    }

    char buf[256];
    if (::read(fd.get(), buf, sizeof buf) <= 0) {
        return false;                  // destructor closes on this path...
    }

    if (generation % 2 == 0) {
        return true;                   // ...and on this one...
    }

    // ... pretend to re-parse buf into the live config here ...
    return true;                       // ...and on this one. Free.
}

int main() {
    const int baseline = count_open_fds();

    for (int gen = 0; gen < 8; ++gen) {
        assert(reload_config(gen));
    }
    assert(count_open_fds() == baseline);

    {
        FdGuard a(::open(kConfigPath, O_RDONLY));
        assert(a.get() >= 0);

        FdGuard b(std::move(a));
        assert(a.get() == -1);
        assert(b.get() >= 0);
        assert(count_open_fds() == baseline + 1);
    }
    assert(count_open_fds() == baseline);
    return 0;
}
