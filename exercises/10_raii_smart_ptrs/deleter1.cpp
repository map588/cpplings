// deleter1.cpp
//
// unique_ptr's destructor doesn't have to call `delete` — the DELETER is
// a template parameter. That makes unique_ptr a universal RAII adapter
// for C-style APIs: files, sockets, mmaps, library handles... anything
// with an open/close pair, wrapped in one line instead of a hand-rolled
// guard class (raii1 — never again).
//
// Two ways to spell the deleter, with a real size difference:
//
//   // function pointer: stored per-instance → TWO pointers wide
//   std::unique_ptr<Conn, void(*)(Conn*)> p(legacy_open(), &legacy_close);
//
//   // stateless functor: empty type, optimized away → ONE pointer wide
//   struct ConnCloser {
//       void operator()(Conn* c) const { legacy_close(c); }
//   };
//   std::unique_ptr<Conn, ConnCloser> p(legacy_open());
//
// The functor wins: same size as a raw pointer, and the close call can
// inline. (The empty deleter takes no storage — empty-base optimization.
// Lambdas work too since they're functors, but a named struct reads
// better in an alias.)
//
// Task: finish ConnCloser and ConnPtr, then convert fetch() to use them.
// The static_asserts pin down the size story.

#include <cassert>
#include <memory>

// --- the "C library" we don't control --------------------------------
struct Conn { int id; };
inline int open_conns = 0;
Conn* legacy_open()           { ++open_conns; return new Conn{open_conns}; }
void legacy_close(Conn* c)    { if (c) { --open_conns; delete c; } }
int legacy_query(Conn* c, int x) { return c->id * 0 + x * 2; }
// ----------------------------------------------------------------------

struct ConnCloser {
    void operator()(Conn* c) const {
        // TODO
    }
};

using ConnPtr = std::unique_ptr<Conn, ConnCloser>;

static_assert(sizeof(ConnPtr) == sizeof(Conn*),
              "a stateless functor deleter costs nothing");
static_assert(sizeof(std::unique_ptr<Conn, void (*)(Conn*)>) ==
                  2 * sizeof(Conn*),
              "a function-pointer deleter is stored per instance");

int fetch(int x) {
    Conn* c = legacy_open();
    int result = legacy_query(c, x);
    if (result > 100) {
        return -1;              // leaks the connection (you knew that)
    }
    legacy_close(c);
    return result;
}

int main() {
    assert(fetch(21) == 42);
    assert(open_conns == 0);

    assert(fetch(60) == -1);    // the early-out path
    assert(open_conns == 0);
    return 0;
}

// I AM NOT DONE
