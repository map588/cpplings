// pointers1.cpp — solution

#include <cassert>

struct Entry {
    const char* key;
    int value;
};

Entry table[] = {{"alpha", 1}, {"beta", 2}, {"gamma", 3}};

Entry* find(const char* key) {
    for (Entry& e : table) {
        const char* a = e.key;
        const char* b = key;
        while (*a && *a == *b) { ++a; ++b; }
        if (*a == *b) return &e;
    }
    return nullptr;
}

int lookup_or_zero(const char* key) {
    Entry* e = find(key);
    return e ? e->value : 0;   // nullable pointer → check before deref
}

int main() {
    assert(lookup_or_zero("beta") == 2);
    assert(lookup_or_zero("delta") == 0);
    return 0;
}
