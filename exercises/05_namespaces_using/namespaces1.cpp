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
// Task: main() refers to the nested namespace by paths that don't exist.
// Fix it two ways: full qualification for the first, a namespace alias
// for the rest.

// I AM NOT DONE

namespace audio::dsp {
    constexpr int kSampleRate = 48000;
    constexpr int half_rate() { return kSampleRate / 2; }
}

int main() {
    // `dsp` is not a top-level namespace — it lives inside `audio`.
    static_assert(dsp::kSampleRate == 48000);

    // Introduce `namespace mix = ...;` here, then use it below:
    static_assert(mix::half_rate() == 24000);
    static_assert(mix::kSampleRate - mix::half_rate() == 24000);
    return 0;
}
