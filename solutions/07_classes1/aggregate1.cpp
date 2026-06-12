// aggregate1.cpp — solution

#include <cassert>
#include <string_view>
#include <type_traits>

struct Config {
    std::string_view host = "localhost";
    int port = 8080;
    bool verbose = false;
    int retries = 3;
};

static_assert(std::is_aggregate_v<Config>,
              "keep Config constructor-free: designated init depends on it");

int main() {
    // Designated: names instead of positions — robust against members
    // being added above, and self-documenting at the call site.
    Config server{.port = 9000, .verbose = true};
    assert(server.host == "localhost");
    assert(server.port == 9000);
    assert(server.verbose == true);
    assert(server.retries == 3);

    // Only what differs; NSDMIs cover the rest.
    Config debug{.verbose = true};
    assert(debug.verbose == true);
    assert(debug.port == 8080);
    return 0;
}
