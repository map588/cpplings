// deleter1.cpp — solution

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
        legacy_close(c);
    }
};

using ConnPtr = std::unique_ptr<Conn, ConnCloser>;

static_assert(sizeof(ConnPtr) == sizeof(Conn*),
              "a stateless functor deleter costs nothing");
static_assert(sizeof(std::unique_ptr<Conn, void (*)(Conn*)>) ==
                  2 * sizeof(Conn*),
              "a function-pointer deleter is stored per instance");

int fetch(int x) {
    ConnPtr c(legacy_open());
    int result = legacy_query(c.get(), x);
    if (result > 100) {
        return -1;              // ConnCloser runs right here
    }
    return result;              // ...and here
}

int main() {
    assert(fetch(21) == 42);
    assert(open_conns == 0);

    assert(fetch(60) == -1);
    assert(open_conns == 0);
    return 0;
}
