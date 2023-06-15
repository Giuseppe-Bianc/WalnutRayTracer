#pragma once
#include "ImageGenerator.h"
#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

using namespace Walnut;

class RayTracerLayer : public Walnut::Layer {
public:
    virtual void OnAttach() override {
        image = std::make_shared<Image>("texture.png");
        renderingTime = std::format("Tempo creazione immagine: {:6.4f} ms", time);
    }
    virtual void OnUIRender() override {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
        ImGui::Begin("Viewport");

        ImGui::Image(image->GetDescriptorSet(), {CAST_F(image->GetWidth()), CAST_F(image->GetHeight())});

        if(time != 0.0 && time != -1.0) {
            ImVec2 textSize = ImGui::CalcTextSize(renderingTime.c_str());

            // Calculate the position to center the text horizontally
            float textPosOffset = (190 + (textSize.x * 0.5f));

            // Display the text above the image
            ImGui::SetCursorPos({CAST_F(image->GetWidth() - textPosOffset), CAST_F(image->GetHeight() - 50)});
            ImGui::Text(renderingTime.c_str());
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

private:
    std::shared_ptr<Image> image;
    double time{};
    std::string renderingTime;

public:
    RayTracerLayer(double time) : time(time) {}
};
