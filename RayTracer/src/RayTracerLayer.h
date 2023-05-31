#pragma once
#include "Timer.h"
#include "Walnut/Image.h"
#include "Walnut/Layer.h"
#include "headers.h"
#define LOG_PROGRESS

class RayTracerLayer : public Walnut::Layer {
   public:
    virtual void OnUpdate(float ts) override {}
    virtual void OnUIRender() override {
        ImGui::Begin(VKRT::windowTitle.data());
        ImGui::Image(image->GetDescriptorSet(), ImVec2(CAST_F(image->GetWidth()), CAST_F(image->GetHeight())));
        ImGui::End();
    }

    RayTracerLayer() {
        std::string_view image_name = "texture.png";
        // image_data.resize(VKRT::ST_w * VKRT::ST_h, 0);
        {
            Timer timer("Image loaded in memory");
            image = std::make_shared<Walnut::Image>(image_name);
        }
        RTINFO("rendering imagine {} w:{} h:{}", image_name, image->GetWidth(), image->GetHeight());
    }

   private:
    uint32_t GenerateRandomColor() const noexcept {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0xff000000, 0xffffffff);
        return dis(gen);
    }
    std::shared_ptr<Walnut::Image> image;
    // std::vector<uint32_t> image_data;
};

class ImageGenerator {
   public:
   private:
};

void generatePNGImageData(cv::Mat &image) {
    Timer timer{"Rendering"};
#pragma omp parallel for
    for(int y = VKRT::h - 1; y >= 0; y--) {
#ifdef LOG_PROGRESS
        if((y % 50) == 0) {
            RTINFO("Scanlines remaining : {}", y);
        }
#endif  // LOG_PROGRESS
        auto g = y * invStHMinusOne;
#pragma omp parallel for
        for(int x = 0; x < VKRT::w; x++) {
            auto r = x * invStWMinusOne;

            auto ir = CAST_UC(VKRT::scale * r);
            auto ig = CAST_UC(VKRT::scale * g);
            auto ib = CAST_UC(64.0);  // (VKRT::scale=256.0) * 0,25 = 64.0

            // Impostazione del colore del pixel nell'immagine
            image.at<cv::Vec3b>(y, x) = cv::Vec3b(ib, ig, ir);
        }
    }
}

// Function to generate a PNG image
#pragma optimize("gt", on)
void generatePNGImage(const char *filename) {
    // Creazione di un'immagine nera
    cv::Mat image(VKRT::h, VKRT::w, CV_8UC3, cv::Scalar(0, 0, 0));
    generatePNGImageData(image);
    // Write the image data to a PNG file
    if(!cv::imwrite(filename, image)) {
        // Error handling in case the image writing fails
        RTERROR("Failed to write image file: {}", filename);
        return;
    }

    RTINFO("Image saved to file: {}", filename);
}
