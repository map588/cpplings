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
//   // stateless functor TYPE: the how-to-destroy is baked into the type
//   // itself, so no per-instance storage is needed (empty-base
//   // optimization) → ONE pointer wide, and the close call can inline
//   std::unique_ptr<Conn, ConnCloser> p(legacy_open());
//
// The functor wins: same size as a raw pointer. (Lambdas work too, since
// they're functors, but a named struct reads better in an alias.)
//
// Task: finish ConnCloser, then make fetch() leak-free on both paths.
//   - the file compiles (both static_asserts hold) and all four asserts
//     pass — no connection may outlive its fetch()
//   - no manual legacy_close call remains in fetch()
// Constraints:
//   - ConnCloser stays stateless and ConnPtr stays the alias — the
//     static_asserts pin the size story
//   - keep the early return for result > 100
//   - don't touch the "C library" block, main, or any assert

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
