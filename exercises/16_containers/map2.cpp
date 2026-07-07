// map2.cpp
//
// Four ways to put something in a map, and they do NOT agree about
// existing keys:
//
//   m[k] = v;                  // inserts OR OVERWRITES, silently
//   m.insert({k, v});          // no-op if k exists; returns
//                              // pair{iterator, bool inserted}
//   m.emplace(k, v);           // like insert, constructs in place...
//                              // but may construct the pair even when
//                              // it then throws it away
//   m.try_emplace(k, args...)  // C++17: constructs the VALUE only if
//                              // k is new
//
// The bug below: a settings cache that must KEEP the first value seen
// per key ("first writer wins") was written with operator[] — last
// writer wins instead.
//
// Task: make the cache first-writer-wins, and correct the TODO asserts
// to what really happens.
//   - "theme" stays "dark" even after remember("theme", "light")
//   - for the two insert() calls at the bottom: predict what each
//     reported bool must be and which value the final lookup sees,
//     then fix the three TODO asserts to match (some may already be
//     right) — they must all pass
// Constraints:
//   - remember() stays a single statement: no check-first-then-insert
//     two-step (in concurrent code that pattern has a name: a race)
//   - only the three lines marked TODO may have their values edited;
//     every other assert stays untouched

#include <cassert>
#include <map>
#include <string>

std::map<std::string, std::string> settings;

// First writer wins: later remember()s of the same key must not change it.
void remember(const std::string& key, const std::string& value) {
    settings[key] = value;
}

int main() {
    remember("theme", "dark");
    remember("lang", "en");
    remember("theme", "light");          // too late — must NOT win

    assert(settings["theme"] == "dark");
    assert(settings["lang"] == "en");
    assert(settings.size() == 2);

    // insert() reports what happened — predict the bools:
    auto [it1, fresh1] = settings.insert({"port", "8080"});
    auto [it2, fresh2] = settings.insert({"port", "9090"});
    assert(fresh1 == false);   // TODO
    assert(fresh2 == false);   // TODO
    assert(it2->second == "9090");   // TODO: which value survived?
    return 0;
}

// I AM NOT DONE
