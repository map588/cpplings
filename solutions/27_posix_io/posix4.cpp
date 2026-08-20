// posix4.cpp — SOLUTION
//
// Each side closes the pipe ends it doesn't use, straight after the
// fork. With the parent's write end gone, the child's close() is the
// last writer leaving — the parent's read loop gets its EOF. waitpid
// then collects the exit status the child reported through _exit.

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
        ::close(fds[0]);                  // the child never reads

        ssize_t n = ::write(fds[1], msg, static_cast<size_t>(len));
        assert(n == len);
        ::close(fds[1]);                  // this writer is done
        ::_exit(static_cast<int>(n));     // report bytes written (8 bits!)
    }

    // ----- parent: the reader -----
    ::close(fds[1]);                      // drop OUR write end, or the
                                          // loop below waits on ourselves

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
    assert(::waitpid(pid, &status, 0) == pid);

    assert(WIFEXITED(status));            // ended by _exit, not a signal
    assert(WEXITSTATUS(status) == len);   // the child's report came through
    return 0;
}
