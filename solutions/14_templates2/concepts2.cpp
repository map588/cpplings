// concepts2.cpp — solution

#include <concepts>
#include <string>

template <typename T>
concept Renderable = requires(T t) {
    { t.render() } -> std::convertible_to<std::string>;
    { t.layer() } -> std::same_as<int>;
};

// --- types to test against --------------------------------------------
struct Sprite {
    std::string render() const { return "sprite"; }
    int layer() const { return 1; }
};

struct Backdrop {
    const char* render() const { return "backdrop"; }
    int layer() const { return 0; }
};

struct Sound {
    int volume() const { return 11; }
};

struct Glitch {
    std::string render() const { return "?"; }
    double layer() const { return 0.5; }
};
// ----------------------------------------------------------------------

static_assert(Renderable<Sprite>);
static_assert(Renderable<Backdrop>);
static_assert(!Renderable<Sound>);
static_assert(!Renderable<Glitch>);

int main() { return 0; }
