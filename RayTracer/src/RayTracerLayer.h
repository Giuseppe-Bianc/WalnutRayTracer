#pragma once
#include "ImageGenerator.h"
#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"

using namespace Walnut;

class RayTracerLayer : public Walnut::Layer {
public:
    virtual void OnUIRender() override {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
        ImGui::Begin("Settings");
        ImGui::Text("Last render: %.3fms", m_LastRenderTime);
        if(ImGui::Button("Render")) {
            Render();
        }
        if(ImGui::Button("Render on image")) {
            RenderOnImage();
        }
        ImGui::End();

        ImGui::Begin("Viewport");

        m_ViewportWidth = ImGui::GetContentRegionAvail().x;
        m_ViewportHeight = ImGui::GetContentRegionAvail().y;

        auto image = m_Renderer.GetFinalImage();
        if(image)
            ImGui::Image(image->GetDescriptorSet(), {CAST_F(image->GetWidth()), CAST_F(image->GetHeight())}, {0, 1}, {1, 0});

        ImGui::End();
        ImGui::PopStyleVar();

        Render();
    }

    inline void RenderOnImage() {
        m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
        m_Renderer.RenderOnImage();
    }

    void Render() {
        Timer timer;
        m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
        m_Renderer.Render();
        m_LastRenderTime = timer.ElapsedMillis();
    }

private:
    Renderer m_Renderer;
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

    float m_LastRenderTime = 0.0f;
};
