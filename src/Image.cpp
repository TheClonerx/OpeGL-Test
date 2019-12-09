#include <Image.hpp>
#include <cstring>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// #include <mutex> stb is not thread safe

bool tcx::Image::loadFromFile(std::string_view path)
{
    int x, y, channels;
    const uint8_t* data = stbi_load(path.data(), &x, &y, &channels, 4);
    if (!data) {
        std::cerr << "ERROR:STB:" << stbi_failure_reason() << '\n';
        return false;
    }

    auto pixels = std::make_unique<uint8_t[]>(x * y * 4);
    std::memcpy(pixels.get(), data, x * y * 4);
    std::swap(m_data, pixels);
    m_size = { x, y };
    return true;
}

glm::ivec2 tcx::Image::size() const noexcept
{
    return m_size;
}

const uint8_t* tcx::Image::data() const noexcept
{
    return m_data.get();
}

uint8_t* tcx::Image::data() noexcept
{
    return m_data.get();
}

void tcx::Image::flipVertically()
{
    if (!m_data)
        return;
    size_t row_size = m_size.x * 4;

    auto top = m_data.get();
    auto bottom = m_data.get() + m_size.x * m_size.y * 4 - row_size;

    for (size_t y = 0; y < m_size.y / 2; ++y) {
        std::swap_ranges(top, top + row_size, bottom);
        top += row_size;
        bottom -= row_size;
    }
}