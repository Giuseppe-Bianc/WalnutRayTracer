#pragma once
#include "Timer.h"
#include "headers.h"
#ifdef _DEBUG
#define LOG_PROGRESS
#endif

class ImageGenerator {
public:
// Function to generate a PNG image
#pragma optimize("gt", on)
    static inline void generatePNGImage(const char *filename) {
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

private:
    static void generatePNGImageData(cv::Mat &image) {
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
};
