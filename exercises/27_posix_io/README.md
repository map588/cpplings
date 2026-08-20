# 27_posix_io — file descriptors, unistd.h, and pipes

Not ISO C++ — POSIX, the substrate under every Unix program (Linux
and macOS alike). Module 10 taught how to *own* a file descriptor;
this module teaches what fds *are* and how processes talk through
them. Everything here compiles on Linux and macOS; the module is
skipped elsewhere.

## What this module covers

- **The five syscalls** — `open`/`read`/`write`/`lseek`/`close`
  (`<unistd.h>`, `<fcntl.h>`). The short-count contract: `read` and
  `write` return how much they actually moved, and robust code loops
  until done. `-1` + `errno` for failures (module 20's discipline).
- **The fd table** — fds are indexes into a per-process table; 0/1/2
  are stdin/stdout/stderr by convention only. `dup`/`dup2` rearrange
  the table — how the shell implements `program > file`, and how you
  capture your own output. Plus the stdio-buffer/fd boundary
  (`fflush` before you swap fd 1 back).
- **`pipe()`** — a kernel byte channel between two fds. The EOF
  contract: a reader sees end-of-file only when *every* write end in
  *every* process is closed — including the reader's own copy. The
  writer-side mirror: `write` on a fully-closed-reader pipe →
  `SIGPIPE`, or `-1`/`EPIPE` once the signal is ignored.
- **`fork()` + `waitpid()`** — the fd table is copied across `fork`,
  so each side must close the ends it doesn't use, or the EOF never
  comes. Children report through the 8-bit exit status; `_exit` in
  the child, `WIFEXITED`/`WEXITSTATUS` in the parent.
- **`fcntl` + `O_NONBLOCK`** — read-modify-write the fd flags (the
  module-01 register discipline again); `EAGAIN` as "try again
  later", the errno that isn't an error.
- **Capstone** — a two-pipe parent↔child pipeline: `echo msg | tr
  a-z A-Z` with you playing the shell.

## Version notes

None of this is versioned C++ — it is POSIX.1, stable since before
C++ existed. The C++ that appears (RAII for fds, `std::filesystem`
neighbours) is tagged where it is used.
