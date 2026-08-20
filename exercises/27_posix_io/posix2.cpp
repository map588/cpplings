// posix2.cpp
//
// Every process owns a TABLE of file descriptors. Three are wired up
// before main() runs: 0 = stdin, 1 = stdout, 2 = stderr (the header
// gives them names: STDIN_FILENO etc.). Nothing about fd 1 is
// special — it is just the slot the C library's stdout writes into.
//
// Two calls rearrange the table:
//
//     dup(old)        →  a NEW fd, lowest free slot, same open file
//     dup2(old, new)  →  make slot `new` point where `old` points
//                        (silently closing what `new` held before)
//
// This is how `program > out.txt` works: the shell opens out.txt,
// dup2()s it onto fd 1, and the program prints "to the terminal"
// none the wiser. Below we do the shell's job ourselves, capturing
// this process's own printf output into a file — the standard trick
// for testing code that insists on printing.
//
// One trap is waiting: printf writes into a USERSPACE buffer that
// stdio flushes to fd 1 later. Swap fd 1 back before the flush and
// the text lands on the terminal after all. fflush(stdout) forces
// the buffer down BEFORE the swap-back.
//
// Task: implement the redirect / restore dance in main.
//   1. save a duplicate of the current fd 1        (dup)
//   2. point fd 1 at capture_fd                    (dup2)
//   3. after printing: flush, then restore fd 1    (fflush, dup2)
//   4. close the two fds you made; every assert passes
// Constraints:
//   - count_open_fds and the asserts stay as they are
//   - the printf line stays exactly as it is — it must not know
//     anything unusual is happening to it

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

    // TODO(1): remember where fd 1 currently points.
    int saved_stdout = -1;

    // TODO(2): point fd 1 at capture_fd.

    printf("hello, capture\n");           // "to the terminal"... or is it?

    // TODO(3): flush stdio, then point fd 1 back at saved_stdout.

    // TODO(4): close saved_stdout and capture_fd — dup2 made fd 1 its
    // own reference to the file; ours is no longer needed.

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

// I AM NOT DONE
