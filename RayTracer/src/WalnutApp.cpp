#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "headers.h"
#include "rstb_image.h"
#include <opencv2/opencv.hpp>
//#define LOG_PROGRESS
class Timer {
public:
	explicit Timer(std::string_view msg) : message(msg) {
		startTimePoint = std::chrono::high_resolution_clock::now();
	}

	#pragma optimize("gt", on)
	inline void Stop() noexcept {
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(startTimePoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();
		auto duration = end - start;

		auto minutes = duration / kNanoSecondsInMinute;
		auto seconds = (duration % kNanoSecondsInMinute) / kNanoSecondsInSecond;
		auto milliseconds = (duration % kNanoSecondsInSecond) / kNanoSecondsInMillisecond;
		auto microseconds = (duration % kNanoSecondsInMillisecond) / kNanoSecondsInMicrosecond;
		auto nanoseconds = duration % kNanoSecondsInMicrosecond;
		//RTINFO("Rendering done in: {}.{:02d}.{:03d}.{:03d}.{:03d}", minutes, seconds, milliseconds, microseconds, nanoseconds);
		RTINFO("{} done in: {}:{:02d}.{:03d}.{:03d}.{:03d}", message, minutes, seconds, milliseconds, microseconds, nanoseconds);
	}
	~Timer() {
		Stop();
	}

private:
	std::string_view message;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
};

class RayTracerLayer : public Walnut::Layer
{
public:
	virtual void OnUpdate(float ts) override {
	}
	virtual void OnUIRender() override
	{
		ImGui::Begin(VKRT::windowTitle.data());
		ImGui::Image(image->GetDescriptorSet(), ImVec2(CAST_F(image->GetWidth()), CAST_F(image->GetHeight())));
		ImGui::End();
	}

	RayTracerLayer() {
		//image_data.resize(VKRT::ST_w * VKRT::ST_h, 0);
		{
			Timer timer("Image loaded in memory");
			image = std::make_shared<Walnut::Image>("texture.png");
		}
	}

private:
	uint32_t GenerateRandomColor() const noexcept {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<uint32_t> dis(0xff000000, 0xffffffff);
		return dis(gen);
	}
	std::shared_ptr<Walnut::Image> image;
	//std::vector<uint32_t> image_data;
};

// Function to generate a PNG image with random pixels
#pragma optimize("gt", on)
void generatePNGImage(const char* filename) {
	// Creazione di un'immagine nera
	cv::Mat image(VKRT::h, VKRT::w, CV_8UC3, cv::Scalar(0, 0, 0));
	{
		Timer timer{ "Rendering" };
#pragma omp parallel for
		for (int y = VKRT::h - 1; y >= 0; y--) {
#ifdef LOG_PROGRESS
			if ((y % 50) == 0) {
				RTINFO("Scanlines remaining : {}", j);
			}
#endif // LOG_PROGRESS
			auto g = y * invStHMinusOne;
#pragma omp parallel for
			for (int x = 0; x < VKRT::w; x++) {
				auto r = x * invStWMinusOne;

				auto ir = CAST_UC(256 * r);
				auto ig = CAST_UC(256 * g);
				auto ib = CAST_UC(64.0); // 256.0 * 0,25

				// Impostazione del colore del pixel nell'immagine
				image.at<cv::Vec3b>(y, x) = cv::Vec3b(ib, ig, ir);
			}
		}
	}
	// Write the image data to a PNG file
	if (!cv::imwrite(filename, image)) {
		// Error handling in case the image writing fails
		RTERROR("Failed to write image file: {}", filename);
		return;
	}

	RTINFO("Image saved to file: {}", filename);
}

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
	auto console = spdlog::stdout_color_mt("console");
	spdlog::set_default_logger(console);
	generatePNGImage("texture.png");
	Walnut::ApplicationSpecification spec;
	spec.Name = VKRT::windowTitle.data();
	spec.Width = 1200;
	spec.Height = 675;

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<RayTracerLayer>();
	return app;
}