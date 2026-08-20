// posix6.cpp — module capstone
//
// The full shell-pipeline mechanism, built by hand: a parent process
// streams text to a child through one pipe, the child transforms it
// and streams the result back through a second pipe. This is
// `echo msg | tr a-z A-Z` with you playing the shell:
//
//     parent ── up pipe ──▶ child (toupper) ── down pipe ──▶ parent
//
// Everything the module taught has a job here:
//   posix1: loop on short counts (write_all is provided; the child's
//           read loop is yours)
//   posix3: a reader's EOF arrives only when every write end is
//           closed — count them per pipe, per process
//   posix4: fork duplicates the fd table (each process starts with
//           ALL FOUR pipe ends), the child reports via _exit /
//           waitpid
//   posix5: check every return value; the byte count is the protocol
//
// The fd hygiene is written out below and commented — read each
// close() and say which rule it serves. Your work is the child's
// filter loop.
//
// Task: implement the child's read → toupper → write loop.
//   - read from up[0] until EOF
//   - uppercase each chunk, send it on with write_all
//   - then close the child's ends and _exit(0)
// Constraints:
//   - the child transforms CHUNKS as they arrive — it must not try
//     to buffer "the whole file" first (real filters stream;
//     pretend the input could be gigabytes)
//   - a buffer of 8 bytes is all you get — the loop matters

#include <cassert>
#include <cctype>       // toupper
#include <cstring>
#include <sys/wait.h>   // POSIX: waitpid, WIFEXITED, WEXITSTATUS
#include <unistd.h>     // POSIX: fork, pipe, read, write, close, _exit

// posix1's loop, ready to use.
ssize_t write_all(int fd, const char* buf, size_t len) {
    size_t done = 0;
    while (done < len) {
        ssize_t n = ::write(fd, buf + done, len - done);
        if (n < 0) {
            return -1;
        }
        done += static_cast<size_t>(n);
    }
    return static_cast<ssize_t>(done);
}

int main() {
    const char msg[] = "pipelines are just fds and discipline";

    int up[2];      // parent → child
    int down[2];    // child → parent
    assert(::pipe(up) == 0);
    assert(::pipe(down) == 0);

    pid_t pid = ::fork();
    assert(pid >= 0);

    if (pid == 0) {
        // ----- child: the filter -----
        ::close(up[1]);       // child never writes upstream   (posix4)
        ::close(down[0]);     // child never reads downstream  (posix4)

        char buf[8];          // deliberately tiny — stream, don't slurp
        (void)buf;

        // TODO: read up[0] until EOF; uppercase each chunk
        // (std::toupper on each byte); write_all it to down[1].

        ::close(up[0]);       // done reading
        ::close(down[1]);     // last writer leaving → parent gets EOF
        ::_exit(1);           // TODO: 0 = filter finished cleanly
    }

    // ----- parent: source, then sink -----
    ::close(up[0]);           // parent never reads upstream
    ::close(down[1]);         // parent never writes downstream — and
                              // holding this would deadlock our own
                              // read loop below (posix4's bug)

    assert(write_all(up[1], msg, std::strlen(msg)) >= 0);
    ::close(up[1]);           // writer done → child gets EOF (posix3)

    char result[64] = {};
    size_t total = 0;
    for (;;) {
        ssize_t n = ::read(down[0], result + total,
                           sizeof result - 1 - total);
        assert(n >= 0);
        if (n == 0) {
            break;
        }
        total += static_cast<size_t>(n);
    }
    ::close(down[0]);

    int status = 0;
    assert(::waitpid(pid, &status, 0) == pid);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    assert(std::strcmp(result, "PIPELINES ARE JUST FDS AND DISCIPLINE") == 0);
    return 0;
}

// I AM NOT DONE
