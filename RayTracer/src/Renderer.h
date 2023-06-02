#pragma once
#include "Walnut/Image.h"
#include "headers.h"

class Renderer {
public:
    Renderer() = default;
    //~Renderer() {}

    void OnResize(uint32_t width, uint32_t height);
    void Render();
    void RenderOnImage() const noexcept;

    std::shared_ptr<Walnut::Image> GetFinalImage() const noexcept { return m_FinalImage; }

private:
    uint32_t PerPixel(glm::vec2 coord) const noexcept;

private:
    std::size_t aspectratio{};
    std::string_view image_name = "texture.png";
    std::shared_ptr<Walnut::Image> m_FinalImage;
    uint32_t *m_ImageData = nullptr;
};
