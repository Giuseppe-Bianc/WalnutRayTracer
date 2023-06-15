#pragma once
#include "headers.h"
namespace WLRT {
    class Timer {
    public:
        explicit Timer() noexcept = default;
        explicit Timer(std::string_view msg) noexcept : message(msg) {
            startTimePoint = std::chrono::high_resolution_clock::now();
        }
        Timer(const Timer &) = delete;

        // Delete the reference copy constructor
        Timer(Timer &&) = delete;

        // Delete the reference reference copy constructor
        Timer &operator=(const Timer &) = delete;
        bool operator==(const Timer &other) = delete;
        ~Timer() {
            const auto endTimePoint = std::chrono::high_resolution_clock::now();
            auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(startTimePoint).time_since_epoch().count();
            auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimePoint).time_since_epoch().count();
            auto duration = end - start;

            auto minutes = duration / kNanoSecondsInMinute;
            auto seconds = (duration % kNanoSecondsInMinute) / kNanoSecondsInSecond;
            auto milliseconds = (duration % kNanoSecondsInSecond) / kNanoSecondsInMillisecond;
            auto microseconds = (duration % kNanoSecondsInMillisecond) / kNanoSecondsInMicrosecond;
            auto nanoseconds = duration % kNanoSecondsInMicrosecond;
            // RTINFO("Rendering done in: {}.{:02d}.{:03d}.{:03d}.{:03d}", minutes, seconds, milliseconds, microseconds,
            // nanoseconds);
            VKINFO("{} done in: {}:{:02d}.{:03d}.{:03d}.{:03d}", message, minutes, seconds, milliseconds, microseconds,
                   nanoseconds);
        }

    private:
        std::string_view message;
        std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
    };

}  // namespace WLRT
