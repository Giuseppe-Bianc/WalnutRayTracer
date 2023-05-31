#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "ImageGenerator.h"
#include "RayTracerLayer.h"
#include "Timer.h"
#include "Walnut/Image.h"
#include "headers.h"

Walnut::Application *Walnut::CreateApplication(int argc, char **argv) {
    spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
    auto console = spdlog::stdout_color_mt("console");
    spdlog::set_default_logger(console);
    ImageGenerator::generatePNGImage("texture.png");
    Walnut::ApplicationSpecification spec;
    spec.Name = VKRT::windowTitle.data();
    spec.Width = VKRT::wind_w;
    spec.Height = VKRT::wind_h;
    RTINFO("crazione finestra {} w:{} h:{}", spec.Name, spec.Width, spec.Height);

    Walnut::Application *app = new Walnut::Application(spec);
    app->PushLayer<RayTracerLayer>();
    return app;
}
