// posix6.cpp — SOLUTION
//
// The child is the standard streaming filter: read a chunk, transform
// it in place, pass it on; EOF from upstream ends the loop. Eight
// bytes of buffer is plenty — the loop, not the buffer, is the
// mechanism.

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
        for (;;) {
            ssize_t n = ::read(up[0], buf, sizeof buf);
            if (n < 0) {
                ::_exit(2);   // read failed
            }
            if (n == 0) {
                break;        // upstream EOF: the parent closed up[1]
            }
            for (ssize_t i = 0; i < n; ++i) {
                buf[i] = static_cast<char>(
                    std::toupper(static_cast<unsigned char>(buf[i])));
            }
            if (write_all(down[1], buf, static_cast<size_t>(n)) < 0) {
                ::_exit(3);   // write failed
            }
        }

        ::close(up[0]);       // done reading
        ::close(down[1]);     // last writer leaving → parent gets EOF
        ::_exit(0);           // filter finished cleanly
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
