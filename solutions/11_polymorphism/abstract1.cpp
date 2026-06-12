// abstract1.cpp — solution

#include <cassert>
#include <memory>
#include <string>
#include <vector>

struct Exporter {
    virtual std::string serialize(int value) const = 0;   // no fallback
    virtual ~Exporter() = default;
};

struct JsonExporter : Exporter {
    std::string serialize(int value) const override {
        return "{\"value\": " + std::to_string(value) + "}";
    }
};

struct YamlExporter : Exporter {
    // With serialize pure, omitting this didn't compile — exactly the
    // pressure we wanted.
    std::string serialize(int value) const override {
        return "value: " + std::to_string(value);
    }
};

int main() {
    std::vector<std::unique_ptr<Exporter>> exporters;
    exporters.push_back(std::make_unique<JsonExporter>());
    exporters.push_back(std::make_unique<YamlExporter>());

    assert(exporters[0]->serialize(7) == "{\"value\": 7}");
    assert(exporters[1]->serialize(7) == "value: 7");
    return 0;
}
