#pragma once

#include <source_location>
#include <fmt/color.h>
#include <fmt/chrono.h>
#include <string>
#include <chrono>
#include <filesystem>

namespace TR {

enum LogLevel {
   Trace,
   Info,
   Warn,
   Error,
};

#define TR_TRACE(format, ...)                                      \
   TR::LogMessage(LogLevel::Trace, "Trace", fg(fmt::color::white), \
                  std::source_location::current(), format __VA_OPT__(, ) __VA_ARGS__)
#define TR_INFO(format, ...)                                      \
   TR::LogMessage(LogLevel::Info, "Info ", fg(fmt::color::white), \
                  std::source_location::current(), format __VA_OPT__(, ) __VA_ARGS__)
#define TR_WARN(format, ...)                                       \
   TR::LogMessage(LogLevel::Warn, "Warn ", fg(fmt::color::yellow), \
                  std::source_location::current(), format __VA_OPT__(, ) __VA_ARGS__)
#define TR_ERROR(format, ...)                                    \
   TR::LogMessage(LogLevel::Error, "Error", fg(fmt::color::red), \
                  std::source_location::current(), format __VA_OPT__(, ) __VA_ARGS__)

template<class... Args>
void LogMessage(LogLevel level, std::string_view levelStr, fmt::text_style color,
                std::source_location loc, std::string_view format, Args &&...args) {
   using namespace std::chrono;
   auto ts = time_point_cast<milliseconds>(system_clock::now());
   auto loc_view = std::filesystem::path(loc.file_name()).make_preferred().string();
   size_t pos =
       loc_view.rfind(std::filesystem::path("/Tremble/").make_preferred().string());
   auto substring = loc_view.substr(pos + 9);

   fmt::print(color, "[{}] [{:%Y/%m/%d %H.%M.%S}] [{}:{}]: ", levelStr, ts, substring,
              loc.line());
   fmt::print(color, format, std::forward<Args>(args)...);
   fmt::println("");
   std::fflush(stdout);
}

} // namespace TR