#pragma once
#include "Timer.h"
#include "Walnut/Image.h"
#include "Walnut/Layer.h"
#include "headers.h"

class RayTracerLayer : public Walnut::Layer {
public:
    virtual void OnUpdate(float ts) override {}
    virtual void OnUIRender() override {
        ImGui::Begin(VKRT::windowTitle.data());
        ImGui::Image(image->GetDescriptorSet(), ImVec2(CAST_F(image->GetWidth()), CAST_F(image->GetHeight())));
        ImGui::End();
    }

    RayTracerLayer() {
        loadImage();
        RTINFO("rendering imagine {} w:{} h:{}", image_name, image->GetWidth(), image->GetHeight());
    }

private:
    inline void loadImage() {
        Timer timer("Image loaded in memory");
        image = std::make_shared<Walnut::Image>(image_name);
    }
    uint32_t GenerateRandomColor() const noexcept {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 256);
        return dis(gen);
    }
    std::string_view image_name = "texture.png";
    std::shared_ptr<Walnut::Image> image;
    // std::vector<uint32_t> image_data;
};
