#pragma once
#include <memory>
#include <string_view>

namespace tcx {

class Sound {
public:
    bool loadFromFile(std::string_view);

private:
    std::unique_ptr<int16_t[]> m_samples;
    int m_sample_count;
    int m_sample_rate;
    int m_channels;
};

} // namespace tcx
