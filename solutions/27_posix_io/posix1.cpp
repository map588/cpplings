// posix1.cpp — SOLUTION
//
// write_all / read_all are the canonical short-count loops, and the
// open() flags say what we mean: write only, create if missing,
// truncate what's there.

#include <cassert>
#include <cstdlib>      // mkstemp
#include <cstring>
#include <fcntl.h>      // POSIX: open, O_*
#include <unistd.h>     // POSIX: read, write, close, unlink

// A miserly but POSIX-conforming kernel: never more than 7 bytes out...
ssize_t stingy_write(int fd, const char* buf, size_t len) {
    return ::write(fd, buf, len < 7 ? len : 7);
}

// ...and never more than 5 bytes in.
ssize_t stingy_read(int fd, char* buf, size_t len) {
    return ::read(fd, buf, len < 5 ? len : 5);
}

// Write ALL len bytes, however many calls it takes.
// Returns len, or -1 on the first error.
ssize_t write_all(int fd, const char* buf, size_t len) {
    size_t done = 0;
    while (done < len) {
        ssize_t n = stingy_write(fd, buf + done, len - done);
        if (n < 0) {
            return -1;
        }
        done += static_cast<size_t>(n);
    }
    return static_cast<ssize_t>(done);
}

// Read until end-of-file or the buffer is full.
// Returns the total byte count, or -1 on the first error.
ssize_t read_all(int fd, char* buf, size_t len) {
    size_t done = 0;
    while (done < len) {
        ssize_t n = stingy_read(fd, buf + done, len - done);
        if (n < 0) {
            return -1;
        }
        if (n == 0) {
            break;                        // end-of-file
        }
        done += static_cast<size_t>(n);
    }
    return static_cast<ssize_t>(done);
}

int main() {
    const char msg[] = "forty bytes that will not fit in one go.";
    const size_t len = sizeof(msg) - 1;   // 40, without the '\0'

    char path[] = "/tmp/cpplings_posix1_XXXXXX";
    int tmp = ::mkstemp(path);            // creates the file, gives an fd
    assert(tmp >= 0);
    ::close(tmp);                         // we'll reopen it ourselves

    // Write only; create it if missing; truncate whatever is there.
    int fd = ::open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    assert(fd >= 0);
    assert(write_all(fd, msg, len) == static_cast<ssize_t>(len));
    ::close(fd);

    fd = ::open(path, O_RDONLY);
    assert(fd >= 0);
    char buf[128] = {};
    assert(read_all(fd, buf, sizeof buf) == static_cast<ssize_t>(len));
    assert(std::memcmp(buf, msg, len) == 0);
    ::close(fd);

    ::unlink(path);
    return 0;
}
