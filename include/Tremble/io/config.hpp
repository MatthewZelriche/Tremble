#pragma once

#include <string>
#include <tl/optional.hpp>

#define TOML_HEADER_ONLY 0
#include <toml++/toml.hpp>

#include "Tremble/io/log.hpp"

namespace TR {

struct Subsystems {
   bool window = true;
   bool audio = true;
};

class Config {
  public:
   Config(std::string_view configPath);

   inline Subsystems GetParsedSubsystems() { return subsystems; }

  private:
   template<typename T>
   T ParseValue(std::string_view category, std::string_view name, T defaultVal) {
      return table[category][name]
          .value<T>()
          .or_else([=] {
             TR_WARN("Engine config missing required value {} in category {}, falling "
                     "back on default value {}",
                     name, category, defaultVal);
             return tl::optional<T>(defaultVal);
          })
          .value();
   }
   toml::parse_result table;
   Subsystems subsystems;
};

} // namespace TR