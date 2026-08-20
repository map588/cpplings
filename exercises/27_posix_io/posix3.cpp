// posix3.cpp
//
// pipe() (POSIX, <unistd.h>) makes a one-way byte channel inside the
// kernel and hands you both ends as fds:
//
//     int fds[2];
//     pipe(fds);       // fds[0] = READ end, fds[1] = WRITE end
//                      // (mnemonic: like stdin 0 / stdout 1)
//
// Bytes go in at fds[1], come out at fds[0], in order. The kernel
// buffers a few tens of KB between them. Pipes are how shells build
// `grep | sort | uniq` — but the whole mechanism turns on one rule:
//
//   THE EOF CONTRACT: read() on a pipe returns 0 ("end of file")
//   only when EVERY write end in EVERY process is closed. While any
//   write end is open — even your own, even one you forgot — a
//   drained pipe doesn't say EOF; it BLOCKS, waiting for data that
//   can never be promised.
//
// The shipped code below is the classic form of the bug: the writer
// is done, the drain loop reads everything... and then hangs forever
// on a pipe that is empty but not ended. (The watcher's 30-second
// timeout is your failure signal — in production it would be a stuck
// service at 3am.)
//
// And the mirror-image rule for writers: write() on a pipe whose
// READ end is fully closed raises SIGPIPE, which kills the process
// by default. Programs that talk to pipes/sockets ignore the signal
// and get a polite -1/EPIPE errno instead — the last block below
// proves that contract.
//
// Task:
//   - end the pipe properly so the drain loop can terminate
//   - fill in the two TODO quiz values in the EPIPE block
// Constraints:
//   - the drain loop itself stays as it is — it is correct;
//     the bug is in what happens (or doesn't) before it

#include <cassert>
#include <cerrno>
#include <csignal>      // signal, SIGPIPE, SIG_IGN
#include <cstring>
#include <unistd.h>     // POSIX: pipe, read, write, close

constexpr ssize_t TODO = -2;

int main() {
    int fds[2];
    assert(::pipe(fds) == 0);

    // The writer's half: three messages, then the writer is done.
    const char* chunks[] = {"alpha ", "beta ", "gamma"};
    for (const char* c : chunks) {
        ssize_t n = ::write(fds[1], c, std::strlen(c));
        assert(n == static_cast<ssize_t>(std::strlen(c)));
    }
    // TODO: the writer said it is done — but the PIPE doesn't know.

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
    assert(n == TODO);                    // what does write return?
    assert(errno == TODO);                // and which errno? (EPIPE/EBADF/0)
    ::close(fds2[1]);
    return 0;
}

// I AM NOT DONE
