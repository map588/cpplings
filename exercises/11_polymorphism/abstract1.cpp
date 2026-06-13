// abstract1.cpp
//
// Exporter::serialize below has a "harmless" default body: return "".
// That default just ATE a bug: YamlExporter was added last sprint and
// nobody implemented serialize — every YAML export in the program is
// silently empty. The compiler had no opinion; the base provided a
// fallback, so falling back is legal.
//
// A PURE VIRTUAL function deletes the fallback:
//
//     virtual std::string serialize(int value) const = 0;
//
// `= 0` means: no default here; this class is ABSTRACT (cannot be
// instantiated); every concrete derived class MUST implement this or
// become abstract itself. "Forgot to implement" is now a compile error
// at the point of instantiation — the bug can't survive to runtime.
//
// (Detail worth knowing: a pure virtual can still HAVE a body, defined
// out of line — derived overrides may call Exporter::serialize(...)
// explicitly for shared logic. Rare, but it confuses people on first
// sight. And destructors of abstract bases: still virtual, module rule
// unchanged.)
//
// Task: make serialize pure, then implement the override the compiler
// demands.

#include <cassert>
#include <memory>
#include <string>
#include <vector>

struct Exporter {
    virtual std::string serialize(int value) const { return ""; }
    virtual ~Exporter() = default;
};

struct JsonExporter : Exporter {
    std::string serialize(int value) const override {
        return "{\"value\": " + std::to_string(value) + "}";
    }
};

struct YamlExporter : Exporter {
    // added in a hurry; serialize() never implemented
    // (when you write it: "value: " + std::to_string(value))
};

int main() {
    std::vector<std::unique_ptr<Exporter>> exporters;
    exporters.push_back(std::make_unique<JsonExporter>());
    exporters.push_back(std::make_unique<YamlExporter>());

    assert(exporters[0]->serialize(7) == "{\"value\": 7}");
    assert(exporters[1]->serialize(7) == "value: 7");   // "" today
    return 0;
}

// I AM NOT DONE
