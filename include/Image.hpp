#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <string_view>

namespace tcx {

class Image {
public:
    Image() noexcept = default;

    bool loadFromFile(std::string_view);

    glm::ivec2 size() const noexcept;
    uint8_t* data() noexcept;
    const uint8_t* data() const noexcept;

    void flipVertically();

private:
    std::unique_ptr<uint8_t[]> m_data = nullptr;
    glm::ivec2 m_size;
};

} // namespace tcx
