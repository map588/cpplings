// posix4.cpp
//
// fork() (POSIX, <unistd.h>) clones the calling process. After it
// returns you have TWO processes running this same code:
//
//     pid_t pid = fork();
//     if (pid == 0)  { /* child:  fork returned 0 */ }
//     else           { /* parent: fork returned the child's pid */ }
//
// The child gets a COPY of the parent's fd table — every open fd,
// including BOTH ends of any pipe. That's the feature that lets a
// pipe cross the process boundary... and the trap: posix3's EOF
// contract counts write ends in EVERY process. Here the child writes
// and the parent reads — but the parent also still holds the write
// end it forked with. Count the writers the pipe sees: the child's
// (closed when it finishes) and the parent's own, held open while
// the parent sits in read() waiting for an EOF that its own fd
// table is preventing. (The 30s timeout is the bug firing.) Rule of
// thumb: first thing after fork, each side closes the ends it
// doesn't use.
//
// The child reports its result the classic Unix way: as its EXIT
// STATUS. The parent collects it with waitpid() (<sys/wait.h>):
//
//     int status;
//     waitpid(pid, &status, 0);          // blocks until the child ends
//     WIFEXITED(status)                  // ended normally?
//     WEXITSTATUS(status)                // its exit code — 8 bits only!
//
// (The child leaves via _exit(code): straight to the kernel, no
// atexit handlers, no flushing of the stdio buffers it shares copies
// of with the parent. In a fork()ed child that's what you want.)
//
// Task:
//   - close the right pipe ends on each side of the fork
//   - finish the parent: collect the child's status, assert the contract
// Constraints:
//   - the child reports through its exit status only

#include <cassert>
#include <cstring>
#include <sys/wait.h>   // POSIX: waitpid, WIFEXITED, WEXITSTATUS
#include <unistd.h>     // POSIX: fork, pipe, read, write, close, _exit

int main() {
    const char msg[] = "a pipe carries bytes, not structure";
    const ssize_t len = static_cast<ssize_t>(std::strlen(msg));

    int fds[2];
    assert(::pipe(fds) == 0);

    pid_t pid = ::fork();
    assert(pid >= 0);

    if (pid == 0) {
        // ----- child: the writer -----
        // TODO: this side never reads — close the end it won't use.

        ssize_t n = ::write(fds[1], msg, static_cast<size_t>(len));
        assert(n == len);
        ::close(fds[1]);                  // this writer is done
        ::_exit(static_cast<int>(n));     // report bytes written (8 bits!)
    }

    // ----- parent: the reader -----
    // TODO: this side never writes — but it HOLDS a write end. Count
    // the pipe's writers before you let the loop below run.

    char buf[64] = {};
    size_t total = 0;
    for (;;) {
        ssize_t n = ::read(fds[0], buf + total, sizeof buf - 1 - total);
        assert(n >= 0);
        if (n == 0) {
            break;                        // EOF: no write end left anywhere
        }
        total += static_cast<size_t>(n);
    }
    ::close(fds[0]);
    assert(std::strcmp(buf, msg) == 0);

    int status = 0;
    // TODO: wait for the child and put its status in `status`.

    assert(WIFEXITED(status));            // ended by _exit, not a signal
    assert(WEXITSTATUS(status) == len);   // the child's report came through
    return 0;
}

// I AM NOT DONE
