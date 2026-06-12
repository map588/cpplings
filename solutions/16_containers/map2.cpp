// map2.cpp — solution

#include <cassert>
#include <map>
#include <string>

std::map<std::string, std::string> settings;

void remember(const std::string& key, const std::string& value) {
    settings.try_emplace(key, value);    // constructs only if key is new
}

int main() {
    remember("theme", "dark");
    remember("lang", "en");
    remember("theme", "light");          // no-op, as required

    assert(settings["theme"] == "dark");
    assert(settings["lang"] == "en");
    assert(settings.size() == 2);

    auto [it1, fresh1] = settings.insert({"port", "8080"});
    auto [it2, fresh2] = settings.insert({"port", "9090"});
    assert(fresh1 == true);              // new key: inserted
    assert(fresh2 == false);             // existing key: refused
    assert(it2->second == "8080");       // the first value survived
    (void)it1;
    return 0;
}
