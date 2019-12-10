#include <Sound.hpp>

// oh god oh please no
#include <stb_vorbis.c>

#include <algorithm>
#include <cstring>
#include <iostream>

bool tcx::Sound::loadFromFile(std::string_view path)
{

    int32_t channels, sample_rate;
    int16_t* output;

    int samples = stb_vorbis_decode_filename(path.data(), &channels, &sample_rate, &output);

    if (samples == -1) {
        std::cerr << "ERROR:STB:VORBIS: can't open file " << path << '\n';
        return false;
    } else if (samples == -2) {
        std::cerr << "ERROR:STB:VORBIS: not enough memory\n";
        return false;
    }

    auto buffer = std::make_unique<int16_t[]>(samples);
    std::copy(output, output + samples, buffer.get());

    std::swap(buffer, m_samples);
    m_sample_count = samples;
    m_sample_rate = sample_rate;
    m_channels = channels;
    return true;
}
