// errors2.cpp — solution

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

struct ConnCloser {
    void operator()(Connection* c) const { close_conn(c); }
};

void transmit(const std::string& data) {
    std::unique_ptr<Connection, ConnCloser> conn(open_conn());
    send_packet(conn.get(), data);    // throw → unwinding runs the deleter
}                                     // normal exit → deleter too

int main() {
    transmit("ping");
    assert(open_connections == 0);

    bool threw = false;
    try {
        transmit("a very large payload indeed");
    } catch (const std::length_error&) {
        threw = true;
    }
    assert(threw);
    assert(open_connections == 0);    // unwinding cleaned up
    return 0;
}
