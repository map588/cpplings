// namespaces1.cpp
//
// A namespace is a named scope. Its contents are reachable from outside
// only through qualification (audio::dsp::kSampleRate), a using-something,
// or a NAMESPACE ALIAS:
//
//   namespace dsp = audio::dsp;   // a short local handle on a long path
//
// Aliases are the underused one: they keep call sites short WITHOUT
// flooding the scope with every name in the namespace (the way
// `using namespace` does). You see them constantly with
// `namespace fs = std::filesystem;`.
//
// Note the definition below uses the compact nested form
// `namespace audio::dsp { }` — C++17. Before that you had to write
// `namespace audio { namespace dsp { } }`.
//
// main() below refers to the nested namespace by paths that don't exist.
//
// Task: make every static_assert compile and pass.
//   - fix the first one with full qualification (no alias, no using)
//   - make the remaining two work through an alias named `mix`
// Constraints:
//   - don't change any static_assert line
//   - no using-directives anywhere

namespace audio::dsp {
    constexpr int kSampleRate = 48000;
    constexpr int half_rate() { return kSampleRate / 2; }
}

int main() {
    // `dsp` is not a top-level namespace — it lives inside `audio`.
    static_assert(dsp::kSampleRate == 48000);

    // Something is missing here — the static_asserts below pin down
    // exactly what `mix` has to mean:
    static_assert(mix::half_rate() == 24000);
    static_assert(mix::kSampleRate - mix::half_rate() == 24000);
    return 0;
}

// I AM NOT DONE
