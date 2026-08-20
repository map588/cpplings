// posix5.cpp — SOLUTION
//
// The read-modify-write dance preserves whatever flags the fd
// already carries. Empty non-blocking pipe: -1 + EAGAIN ("try again
// later" — the errno that isn't an error). Three bytes waiting, 64
// requested: 3, a normal partial read.

#include <cassert>
#include <cerrno>
#include <fcntl.h>      // POSIX: fcntl, F_GETFL, F_SETFL, O_NONBLOCK
#include <unistd.h>     // POSIX: pipe, read, write, close

// Turn on O_NONBLOCK without disturbing the fd's other flags.
// Returns true on success.
bool set_nonblocking(int fd) {
    int flags = ::fcntl(fd, F_GETFL);
    if (flags == -1) {
        return false;
    }
    return ::fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1;
}

int main() {
    int fds[2];
    assert(::pipe(fds) == 0);
    assert(set_nonblocking(fds[0]));

    char buf[64];

    // 1) The pipe is empty. A blocking read would park us forever —
    //    a non-blocking one answers immediately: -1, "try again".
    ssize_t n = ::read(fds[0], buf, sizeof buf);
    assert(n == -1);
    assert(errno == EAGAIN || errno == EWOULDBLOCK);

    // 2) Three bytes arrive. We ask for 64 — "give me what's there".
    assert(::write(fds[1], "abc", 3) == 3);
    n = ::read(fds[0], buf, sizeof buf);
    assert(n == 3);                       // fewer than asked ≠ failure

    ::close(fds[0]);
    ::close(fds[1]);
    return 0;
}
