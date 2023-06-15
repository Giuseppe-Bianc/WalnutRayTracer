#pragma once
#include "Timer.h"
#include "Walnut/Timer.h"
#include "headers.h"
#ifdef _DEBUG
#define LOG_PROGRESS
#endif

class ImageGenerator {
public:
// Function to generate a PNG image
#pragma optimize("gt", on)
    static inline double generatePNGImage(const char *filename) {
        // Creazione di un'immagine nera
        cv::Mat image(VKRT::h, VKRT::w, CV_8UC3, cv::Scalar(0, 0, 0));
        double time = generatePNGImageData(image);
        // Write the image data to a PNG file
        if(!cv::imwrite(filename, image)) {
            // Error handling in case the image writing fails
            VKERROR("Failed to write image file: {}", filename);
            return -1.0;
        }

        VKINFO("Image saved to file: {}", filename);
        return time;
    }

// Function to generate a PNG image
#pragma optimize("gt", on)
    static inline void generatePNGImagefromUint32(const char *filename, const uint32_t *imageData, int width, int height) {
        // Creazione di un'immagine nera
        cv::Mat image = convertUint32ArrayToMat(imageData, width, height);
        // Write the image data to a PNG file
        if(!cv::imwrite(filename, image)) {
            // Error handling in case the image writing fails
            VKERROR("Failed to write image file: {}", filename);
            return;
        }

        VKINFO("Image saved to file: {}", filename);
    }

private:
    static double generatePNGImageData(cv::Mat &image) {
#ifdef _DEBUG
        WLRT::Timer timer{"Rendering"};
#else
        Walnut::Timer timer;
#endif
#pragma omp parallel for
        for(int y = VKRT::h - 1; y >= 0; y--) {
#ifdef LOG_PROGRESS
            if((y % 50) == 0) {
                VKINFO("Scanlines remaining : {}", y);
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
#ifdef _DEBUG
        return 0;
#else
        double time = timer.ElapsedMillis();
        return time;
#endif
    }
    static cv::Mat convertUint32ArrayToMat(const uint32_t *imageData, int width, int height) {
        // Creazione di un oggetto cv::Mat con dimensioni e tipo appropriati
        cv::Mat image(height, width, CV_8UC3, cv::Scalar(0, 0, 0));

        // Copia dei dati dall'array uint32_t all'immagine cv::Mat
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                // Ottenimento del valore uint32_t dal puntatore all'array
                uint32_t pixelValue = imageData[y * width + x];

                // Estrazione dei componenti di colore (canali BGR)
                uint8_t blue = (pixelValue >> 0) & 0xFF;
                uint8_t green = (pixelValue >> 8) & 0xFF;
                uint8_t red = (pixelValue >> 16) & 0xFF;

                // Impostazione del valore del pixel nell'immagine cv::Mat
                image.at<cv::Vec3b>(y, x) = cv::Vec3b(blue, green, red);
            }
        }

        return image;
    }
};
