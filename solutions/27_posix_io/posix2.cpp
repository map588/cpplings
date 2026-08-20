// posix2.cpp — SOLUTION
//
// The shell's redirect dance, done to ourselves: dup to save slot 1,
// dup2 to repoint it, fflush BEFORE restoring (stdio's buffer must
// drain while fd 1 still aims at the file), dup2 back, close both
// helper fds.

#include <cassert>
#include <cstdio>
#include <cstdlib>      // mkstemp
#include <cstring>
#include <fcntl.h>      // POSIX: open, fcntl
#include <unistd.h>     // POSIX: dup, dup2, close, read, unlink

// Portable fd census: ask fcntl about every plausible slot. (Linux
// has /proc/self/fd, but this works on macOS/BSD too.)
int count_open_fds() {
    int n = 0;
    for (int fd = 0; fd < 256; ++fd) {
        if (::fcntl(fd, F_GETFD) != -1) {
            ++n;
        }
    }
    return n;
}

int main() {
    const int baseline = count_open_fds();

    char path[] = "/tmp/cpplings_posix2_XXXXXX";
    int capture_fd = ::mkstemp(path);
    assert(capture_fd >= 0);

    // (1) Remember where fd 1 currently points.
    int saved_stdout = ::dup(STDOUT_FILENO);
    assert(saved_stdout >= 0);

    // (2) Point fd 1 at capture_fd.
    assert(::dup2(capture_fd, STDOUT_FILENO) == STDOUT_FILENO);

    printf("hello, capture\n");           // "to the terminal"... or is it?

    // (3) Drain stdio's buffer into the file, THEN restore fd 1.
    fflush(stdout);
    assert(::dup2(saved_stdout, STDOUT_FILENO) == STDOUT_FILENO);

    // (4) fd 1 holds its own reference now; ours can go.
    ::close(saved_stdout);
    ::close(capture_fd);

    // Did the text land in the file?
    int fd = ::open(path, O_RDONLY);
    assert(fd >= 0);
    char buf[64] = {};
    ssize_t got = ::read(fd, buf, sizeof buf - 1);
    assert(got > 0);
    assert(std::strcmp(buf, "hello, capture\n") == 0);
    ::close(fd);
    ::unlink(path);

    // Is stdout really back? (You should see this line on screen.)
    printf("stdout restored\n");
    fflush(stdout);

    assert(count_open_fds() == baseline);     // no fd left behind
    return 0;
}
