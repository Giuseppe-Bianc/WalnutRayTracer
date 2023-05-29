#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "headers.h"
#include "rstb_image.h"


class Timer {
public:
	Timer(std::string_view msg) : message(msg) {
		startTimePoint = std::chrono::high_resolution_clock::now();
	}

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
		ImGui::Begin(VKRT::windowTitle.data(), nullptr, ImGuiWindowFlags_NoDecoration);
		ImGui::Image(image->GetDescriptorSet(), ImVec2((float)image->GetWidth(), (float)image->GetHeight()));
		ImGui::End();
	}

	RayTracerLayer() {
		//image_data.resize(VKRT::ST_w * VKRT::ST_h, 0);
		image = std::make_shared<Walnut::Image>("texture.png");
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
#pragma omp parallel for
#pragma optimize("t", on)
void generatePNGImage(const char* filename)
{
	std::vector<unsigned char> imageData(VKRT::dataSize);
	std::size_t index = 0;
	{
		Timer timer{ "Rendering" };
#pragma omp parallel for
		for (std::size_t j = VKRT::ST_h - 1; j > 0; --j) {
#ifdef LOG_PROGRESS
			if ((j % 50) == 0) {
				RTINFO("Scanlines remaining : {}", j);
			}
#endif // LOG_PROGRESS
			auto g = CAST_D(j) * invStHMinusOne;

			for (std::size_t i = 0; i < VKRT::ST_w; ++i) {
				auto r = CAST_D(i) * invStWMinusOne;
				auto colorX = VKRT::scale * r;
				auto colorY = VKRT::scale * g;
				auto colorZ = 63.999975;
				index = (j * VKRT::ST_w + i) * VKRT::nCh;
				imageData[index] = CAST_UC(colorX);
				imageData[index + 1] = CAST_UC(colorY);
				imageData[index + 2] = CAST_UC(colorZ);
			}
		}
	}


	// Write the image data to a PNG file
	if (stbi_write_png(filename, VKRT::ST_w, VKRT::ST_h, VKRT::nCh, imageData.data(), VKRT::ST_w * VKRT::nCh) == 0) {
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