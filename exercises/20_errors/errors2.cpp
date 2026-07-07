// errors2.cpp
//
// When an exception flies, the runtime UNWINDS: every local object
// between the throw and the catch gets its destructor run, innermost
// scope outward (module 07's ordering, under fire). Locals clean up.
// Things a local was supposed to clean up LATER... don't:
//
//     Connection* conn = open();      // raw owning pointer
//     send(conn, data);               // throws ───────────┐
//     close(conn);                    // never runs ◄──────┘ LEAK
//
// There is no `finally` in C++ — deliberately. The answer is module
// 10's RAII: hold the resource in an object, and unwinding's
// destructor-calls ARE your cleanup. A unique_ptr, a lock_guard, an
// fstream: all exception-safe by construction.
//
// (This is also why destructors must not throw: throwing during an
// unwind — two exceptions in flight — is instant std::terminate.
// That's why dtors are implicitly noexcept.)
//
// transmit() below leaks when send_packet throws. Two honest routes to
// the fix: teach an existing RAII owner how to close THIS kind of
// handle, or write a tiny guard type of your own. Either way, the
// destructor does the closing — on every path.
//
// Task: make transmit() leak-proof.
//   - both open_connections asserts pass (happy path AND unwound path)
//   - the length_error still propagates out to main's catch
// Constraints:
//   - don't touch the "C library" block; connections still come from
//     open_conn and must end in close_conn
//   - no try/catch in transmit — unwinding itself must do the cleanup

#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>

// --- the "C library" -------------------------------------------------
struct Connection { int id; };
inline int open_connections = 0;
Connection* open_conn()        { ++open_connections; return new Connection{1}; }
void close_conn(Connection* c) { if (c) { --open_connections; delete c; } }
void send_packet(Connection*, const std::string& data) {
    if (data.size() > 16) throw std::length_error("packet too big");
}
// ----------------------------------------------------------------------

void transmit(const std::string& data) {
    Connection* conn = open_conn();
    send_packet(conn, data);          // may throw → close never runs
    close_conn(conn);
}

int main() {
    transmit("ping");
    assert(open_connections == 0);    // happy path: fine

    bool threw = false;
    try {
        transmit("a very large payload indeed");
    } catch (const std::length_error&) {
        threw = true;
    }
    assert(threw);
    assert(open_connections == 0);    // unwound path: leaked
    return 0;
}

// I AM NOT DONE
