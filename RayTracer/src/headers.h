#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_WIN32
#define GLM_FORCE_SIZE_T_LENGTH
#define GLM_FORCE_PURE
#define GLM_FORCE_INLINE
#define GLM_FORCE_EXPLICIT_CTOR
#define GLM_FORCE_CXX20
#define GLM_FORCE_UNRESTRICTED_GENTYPE
#define GLM_FORCE_PRECISION_HIGHP_DOUBLE
#define GLM_FORCE_PRECISION_HIGHP_INT
#define GLM_FORCE_PRECISION_HIGHP_FLOAT
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(push)
#pragma warning(disable : 6386 6385)
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <execution>
#include <format>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <numbers>
#include <omp.h>
#include <opencv2/opencv.hpp>
#include <optional>
#include <random>
#include <set>
#include <spdlog/cfg/env.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unordered_set>
#include <vector>
// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning(pop)
#endif

[[nodiscard]] static constexpr auto calcolaCentro(const unsigned int width, const unsigned int w) noexcept {
    return (width - w) / 2;
}
#ifndef _DIST
#define RTTRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define RTDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define RTINFO(...) SPDLOG_INFO(__VA_ARGS__)
#define RTWARN(...) SPDLOG_WARN(__VA_ARGS__)
#define RTERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define RTCRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#else
#define RTTRACE(...)
#define RTDEBUG(...)
#define RTINFO(...)
#define RTWARN(...)
#define RTERROR(...)
#define RTCRITICAL(...)
#endif  // !_DIST

#define CAST_ST(x) static_cast<std::size_t>(x)
#define CAST_CPCU32T(x) static_cast<const uint32_t *>(static_cast<const void *>(x))
#define CAST_UC(x) static_cast<unsigned char>(x)
#define CAST_32T(x) static_cast<int32_t>(x)
#define CAST_U32T(x) static_cast<uint32_t>(x)
#define CAST_U8T(c) static_cast<uint8_t>(x)
#define CAST_U8(x) static_cast<Uint8>(x)
#define CAST_I(x) static_cast<int>(x)
#define CAST_L(x) static_cast<long>(x)
#define CAST_D(x) static_cast<double>(x)
#define CAST_F(x) static_cast<float>(x)
#define CALC_CENTRO(width, w) calcolaCentro((width), (w))
#define PRINT(p, ...) std::cout << std::fixed << std::setprecision(p) << __VA_ARGS__ << std::endl;
#define PRINTNNL(p, ...) std::cout << std::fixed << std::setprecision(p) << __VA_ARGS__;
#define POW2(p) (p) * (p)
#define RTSYSPAUSE()                                                                                                             \
    RTINFO("Press enter to exit...");                                                                                            \
    std::cin.ignore();

using ddvector = std::vector<std::vector<double>>;
namespace VKRT {
    static inline constexpr bool FWDTF = true;
    static inline constexpr bool BCKTF = false;
    static inline constexpr double NO_COLOR = 0.0;
    static inline constexpr double EPSILON = 1e-22;
    static inline constexpr long NANOD = static_cast<long>(std::nano::den);
    static inline constexpr long double pi = std::numbers::pi_v<long double>;
    // Dimensioni della finestra
    // static inline constexpr double aspect_ratio = 16.0 / 9.0;
    static inline constexpr double aspect_ratioW = 16.0;
    static inline constexpr double aspect_ratioH = 9.0;
    static inline constexpr int imageF = 64;
    static inline constexpr int windowF = 68;
    static inline constexpr int w = CAST_I(aspect_ratioW * imageF);
    static inline constexpr int h = CAST_I(aspect_ratioH * imageF);
    static inline constexpr int wind_w = CAST_I(aspect_ratioW * windowF);
    static inline constexpr int wind_h = CAST_I(aspect_ratioH * windowF);
    static inline constexpr int samples_per_pixel = 100;
    static inline constexpr int max_depth = 50;
    static inline constexpr std::size_t ST_w = CAST_ST(w);
    static inline constexpr std::size_t ST_h = CAST_ST(h);
    static inline constexpr std::size_t ST_wind_w = CAST_ST(wind_w);
    static inline constexpr std::size_t ST_wind_h = CAST_ST(wind_h);
    static inline constexpr double scale = 256.0;
    static inline constexpr std::string_view windowTitle = "Ray Tracer";
}  // namespace VKRT
static inline constexpr long long kNanoSecondsInMicrosecond = 1000;
static inline constexpr long long kNanoSecondsInMillisecond = 1000000;
static inline constexpr long long kNanoSecondsInSecond = 1000000000;
static inline constexpr long long kNanoSecondsInMinute = 60000000000;
static inline constexpr double invStHMinusOne = 1.0 / CAST_D(VKRT::ST_h - 1);
static inline constexpr double invStWMinusOne = 1.0 / CAST_D(VKRT::ST_w - 1);
