// namespaces2.cpp
//
// Two tools, very different blast radius:
//
//   using namespace json;     // using-DIRECTIVE: every name in json,
//                             // dumped into this scope
//   using json::serialize;    // using-DECLARATION: exactly one name
//
// The directive's problem isn't style — it's that it composes badly.
// Import two namespaces and the moment they share a name, every
// unqualified use is AMBIGUOUS. Worse, the error appears when a name is
// ADDED to either namespace later: a library update breaks your build (or
// silently changes which function you call, if overload resolution can
// pick a "better" match). This is exactly why `using namespace` in a
// HEADER is forbidden in every style guide: it inflicts that risk on
// every file that includes it.
//
// Below, serialize() is ambiguous. main() wants json's serialize and
// yaml's indent_width.
//
// Task: make the file compile with every assert passing — and make it
// robust: a name added to json or yaml tomorrow must not be able to
// break this file or change what it calls.
// Constraints:
//   - both using-directives must go
//   - don't change any assert line (main keeps using the two names
//     unqualified)

#include <cassert>
#include <string>

namespace json {
    std::string serialize(int v) { return "{" + std::to_string(v) + "}"; }
    constexpr int indent_width = 2;
}

namespace yaml {
    std::string serialize(int v) { return "- " + std::to_string(v); }
    constexpr int indent_width = 4;
}

using namespace json;
using namespace yaml;

int main() {
    assert(serialize(7) == "{7}");      // ambiguous: json:: or yaml::?
    assert(indent_width == 4);          // ambiguous too — we want yaml's
    assert(json::indent_width == 2);    // qualification always works
    return 0;
}

// I AM NOT DONE
