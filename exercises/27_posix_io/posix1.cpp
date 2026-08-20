// posix1.cpp
//
// (POSIX module: everything here is <unistd.h>/<fcntl.h> territory —
// not ISO C++, but the substrate every Unix program stands on. Runs
// on Linux and macOS alike.)
//
// Below iostreams, below stdio, there are exactly five verbs:
//
//     open()   path + flags        →  file descriptor (an int)
//     read()   fd + buffer + size  →  bytes actually read
//     write()  fd + buffer + size  →  bytes actually written
//     lseek()  fd + offset         →  move the file position
//     close()  fd                  →  release it (module 10: RAII!)
//
// The contract everyone learns the hard way: read() and write() may
// do LESS than you asked. write() to a pipe, socket, or interrupted
// call can return after 7 of your 40 bytes; read() gives whatever is
// available now. The return value is not a success flag — it is a
// byte count you must act on. Robust code loops until done:
//
//     while (bytes remain) { r = write(fd, p, remaining); p += r; }
//
// (And -1 means error, with the reason in errno — module 20.)
//
// Regular files rarely short-write, so the bug hides for years and
// surfaces on a socket in production. Here the "kernel" is honest
// about it: stingy_write moves at most 7 bytes per call, stingy_read
// at most 5 — exactly what POSIX permits.
//
// Task: make the round trip work.
//   - fix the open() flags: we want to WRITE, creating/truncating
//   - implement write_all / read_all as loops over the stingy calls
//   - every assert passes
// Constraints:
//   - don't touch stingy_write / stingy_read — they are the kernel
//   - write_all / read_all must handle -1 (return -1 immediately)
//   - read_all stops at end-of-file (a 0 return), not on a full buffer

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
    return stingy_write(fd, buf, len);   // TODO: that was one call, not all
}

// Read until end-of-file or the buffer is full.
// Returns the total byte count, or -1 on the first error.
ssize_t read_all(int fd, char* buf, size_t len) {
    return stingy_read(fd, buf, len);    // TODO: same story
}

int main() {
    const char msg[] = "forty bytes that will not fit in one go.";
    const size_t len = sizeof(msg) - 1;   // 40, without the '\0'

    char path[] = "/tmp/cpplings_posix1_XXXXXX";
    int tmp = ::mkstemp(path);            // creates the file, gives an fd
    assert(tmp >= 0);
    ::close(tmp);                         // we'll reopen it ourselves

    // TODO: these flags ask to READ — write() on this fd returns -1
    // (errno EBADF). We want: write only, truncate to empty.
    int fd = ::open(path, O_RDONLY, 0644);
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

// I AM NOT DONE
