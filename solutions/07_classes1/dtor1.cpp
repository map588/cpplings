// dtor1.cpp — solution

#include <cassert>
#include <string>

struct Probe {
    char tag;
    std::string& log;
    Probe(char t, std::string& l) : tag(t), log(l) { log += tag; }
    ~Probe() { log += '~'; log += tag; }
};

struct Pair {
    Probe right;
    Probe left;
    Pair(std::string& l) : right('r', l), left('l', l) {}
};

int main() {
    {
        std::string log;
        {
            Probe a('a', log);
            Probe b('b', log);
        }
        // Locals: reverse construction order — b dies first.
        assert(log == "ab~b~a");
    }
    {
        std::string log;
        {
            Pair p(log);
        }
        // Members: construct in declaration order (right, left),
        // destroy in reverse (left, right).
        assert(log == "rl~l~r");
    }
    return 0;
}
