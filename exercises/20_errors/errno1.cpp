// errno1.cpp
//
// (POSIX exercise: open/close/write come from <fcntl.h>/<unistd.h>.
// errno itself is standard C, from <cerrno>.)
//
// A syscall can't throw. The kernel reports failure through the return
// value (-1) and leaves WHY in errno — a thread-local int the C library
// maintains. The catch: errno is only meaningful IMMEDIATELY after the
// failed call. It's a shared scratch variable — the very next libc call
// may overwrite it (a failing one certainly will, and even a succeeding
// one is allowed to). The discipline: failed syscall → grab errno NOW.
//
// C++11 gave this world a type system: <system_error>.
//
//     std::error_code ec(errno, std::generic_category());
//
// packages the raw int with the category that knows how to name it, and
// it compares — portably, by name — against the std::errc enumerators:
//
//     ec == std::errc::no_such_file_or_directory    // true for ENOENT
//
// This is the error vocabulary of std::filesystem's non-throwing
// overloads, of networking libraries, and of most embedded and other
// exception-free codebases — module 20's decision table calls this row
// "error codes at the C/ABI boundary".
//
// read_config below gets the discipline wrong: between the failed
// open() and the errno read it calls a logging helper, which performs
// its own (failing) syscall and stomps on errno. The error the caller
// receives describes the LOGGER's problem, not the open()'s.
//
// Task: make read_config report the open() failure faithfully.
//   - a missing file yields std::errc::no_such_file_or_directory, the
//     bad-path case yields std::errc::not_a_directory, message() is
//     non-empty, and the happy path stays error-free
// Constraints:
//   - keep the log_failure call in place and unchanged — real daemons
//     log on failure; your code must be correct anyway
//   - the error_code must be built from errno; don't fabricate the
//     value (the second failure case exists to catch exactly that)
//   - don't change any assert line

#include <cassert>
#include <cerrno>
#include <cstring>
#include <fcntl.h>        // POSIX: open, O_RDONLY
#include <system_error>   // C++11: error_code, errc, generic_category
#include <unistd.h>       // POSIX: close, write

// The daemon's log fd — opened later during startup. Usually.
int g_log_fd = -1;

// Best-effort logging: mirror the message into the log fd; if even that
// fails, there's no one left to tell.
void log_failure(const char* msg) {
    if (::write(g_log_fd, msg, std::strlen(msg)) < 0) {
        // nothing to be done — but this call just had its own errno story
    }
}

// Reads the config file; reports failure as an error_code.
std::error_code read_config(const char* path) {
    int fd = ::open(path, O_RDONLY);
    if (fd < 0) {
        log_failure("read_config: open failed\n");
        return std::error_code(errno, std::generic_category());
        //                     ^^^^^ which call's errno is this, exactly?
    }
    ::close(fd);
    return {};    // success: value 0, tests false
}

int main() {
    std::error_code ec = read_config("/no/such/dir/app.conf");
    assert(ec == std::errc::no_such_file_or_directory);   // ENOENT, by name
    assert(!ec.message().empty());                        // human-readable, free

    // A different failure must map to a different code:
    std::error_code ec2 = read_config("/proc/self/status/nope");
    assert(ec2 == std::errc::not_a_directory);            // ENOTDIR

    // And success is simply "false":
    assert(!read_config("/proc/self/status"));
    return 0;
}

// I AM NOT DONE
