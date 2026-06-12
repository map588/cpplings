// namespaces1.cpp — solution

namespace audio::dsp {
    constexpr int kSampleRate = 48000;
    constexpr int half_rate() { return kSampleRate / 2; }
}

int main() {
    // Full qualification:
    static_assert(audio::dsp::kSampleRate == 48000);

    // Namespace alias — short call sites, no namespace pollution:
    namespace mix = audio::dsp;
    static_assert(mix::half_rate() == 24000);
    static_assert(mix::kSampleRate - mix::half_rate() == 24000);
    return 0;
}
