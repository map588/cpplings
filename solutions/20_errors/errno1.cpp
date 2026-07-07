// errno1.cpp — solution

#include <cassert>
#include <cerrno>
#include <cstring>
#include <fcntl.h>        // POSIX: open, O_RDONLY
#include <system_error>   // C++11: error_code, errc, generic_category
#include <unistd.h>       // POSIX: close, write

// The daemon's log fd — opened later during startup. Usually.
int g_log_fd = -1;

void log_failure(const char* msg) {
    if (::write(g_log_fd, msg, std::strlen(msg)) < 0) {
        // nothing to be done — but this call just had its own errno story
    }
}

std::error_code read_config(const char* path) {
    int fd = ::open(path, O_RDONLY);
    if (fd < 0) {
        const int saved = errno;    // grab it NOW, before any other call
        log_failure("read_config: open failed\n");
        return std::error_code(saved, std::generic_category());
    }
    ::close(fd);
    return {};
}

int main() {
    std::error_code ec = read_config("/no/such/dir/app.conf");
    assert(ec == std::errc::no_such_file_or_directory);
    assert(!ec.message().empty());

    std::error_code ec2 = read_config("/proc/self/status/nope");
    assert(ec2 == std::errc::not_a_directory);

    assert(!read_config("/proc/self/status"));
    return 0;
}
