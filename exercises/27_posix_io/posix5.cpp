// posix5.cpp
//
// By default read() BLOCKS: no data yet means "park this thread
// until there is some". Event-driven programs (servers juggling a
// thousand connections on one thread) can't afford that — they flip
// the fd into NON-BLOCKING mode and treat "nothing available" as an
// ordinary, immediate answer.
//
// The switch lives in fcntl() (<fcntl.h>), the fd Swiss-army knife.
// Mode flags must be read-modify-written, like the device registers
// in module 01 — clobbering flags you didn't mean to touch is the
// classic mistake:
//
//     int flags = fcntl(fd, F_GETFL);         // read current flags
//     fcntl(fd, F_SETFL, flags | O_NONBLOCK); // write them back + ours
//
// The new contract on a non-blocking fd:
//
//     read() with data waiting  →  the bytes available RIGHT NOW —
//                                  possibly fewer than you asked for
//                                  (posix1's loop, still mandatory)
//     read() with none waiting  →  -1, errno == EAGAIN (a.k.a.
//                                  EWOULDBLOCK — same value on
//                                  Linux and macOS)
//
// EAGAIN is the one errno that ISN'T an error — it's the kernel
// saying "try again later". Event loops wait on poll()/kqueue()/
// epoll() and come back; errno1's discipline (check the return
// FIRST, then errno) matters doubly here.
//
// Task:
//   - implement set_nonblocking() with the read-modify-write dance
//   - predict the two quiz TODOs
// Constraints:
//   - set_nonblocking must PRESERVE flags already on the fd
//     (F_GETFL first — no bare fcntl(fd, F_SETFL, O_NONBLOCK))

#include <cassert>
#include <cerrno>
#include <fcntl.h>      // POSIX: fcntl, F_GETFL, F_SETFL, O_NONBLOCK
#include <unistd.h>     // POSIX: pipe, read, write, close

constexpr ssize_t TODO = -2;

// Turn on O_NONBLOCK without disturbing the fd's other flags.
// Returns true on success.
bool set_nonblocking(int fd) {
    (void)fd;
    return false;   // TODO: F_GETFL, then F_SETFL with O_NONBLOCK added
}

int main() {
    int fds[2];
    assert(::pipe(fds) == 0);
    assert(set_nonblocking(fds[0]));

    char buf[64];

    // 1) The pipe is empty. A blocking read would park us forever —
    //    what does a non-blocking one do instead?
    ssize_t n = ::read(fds[0], buf, sizeof buf);
    assert(n == TODO);
    assert(errno == EAGAIN || errno == EWOULDBLOCK);

    // 2) Three bytes arrive. We ask for 64 — "give me what's there".
    assert(::write(fds[1], "abc", 3) == 3);
    n = ::read(fds[0], buf, sizeof buf);
    assert(n == TODO);                    // fewer than asked ≠ failure

    ::close(fds[0]);
    ::close(fds[1]);
    return 0;
}

// I AM NOT DONE
