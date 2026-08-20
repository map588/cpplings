// posix3.cpp — SOLUTION
//
// One line ends the hang: close the write end when the writer is
// done. Only then can a drained pipe report EOF. And a write against
// a fully closed read end (SIGPIPE ignored) returns -1 with EPIPE.

#include <cassert>
#include <cerrno>
#include <csignal>      // signal, SIGPIPE, SIG_IGN
#include <cstring>
#include <unistd.h>     // POSIX: pipe, read, write, close

int main() {
    int fds[2];
    assert(::pipe(fds) == 0);

    // The writer's half: three messages, then the writer is done.
    const char* chunks[] = {"alpha ", "beta ", "gamma"};
    for (const char* c : chunks) {
        ssize_t n = ::write(fds[1], c, std::strlen(c));
        assert(n == static_cast<ssize_t>(std::strlen(c)));
    }
    ::close(fds[1]);      // tell the PIPE the writer is done

    // The reader's half: drain until end-of-file. (This loop is the
    // canonical pipe-reader; don't change it.)
    char buf[64] = {};
    size_t total = 0;
    for (;;) {
        ssize_t n = ::read(fds[0], buf + total, sizeof buf - 1 - total);
        assert(n >= 0);
        if (n == 0) {
            break;                        // EOF: all write ends closed
        }
        total += static_cast<size_t>(n);
    }
    assert(std::strcmp(buf, "alpha beta gamma") == 0);
    ::close(fds[0]);

    // The writer-side contract: read end gone, SIGPIPE ignored.
    ::signal(SIGPIPE, SIG_IGN);
    int fds2[2];
    assert(::pipe(fds2) == 0);
    ::close(fds2[0]);                     // nobody will ever read
    ssize_t n = ::write(fds2[1], "x", 1);
    assert(n == -1);                      // the write fails...
    assert(errno == EPIPE);               // ...with "broken pipe"
    ::close(fds2[1]);
    return 0;
}
