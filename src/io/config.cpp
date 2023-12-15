#define TOML_OPTIONAL_TYPE tl::optional
#define TOML_IMPLEMENTATION
#include "Tremble/io/config.hpp"

#include <exception>

#include "Tremble/io/log.hpp"

using namespace TR;

Config::Config(std::string_view configPath) {
   try {
      table = toml::parse_file(configPath);
      subsystems = {ParseValue("subsystems", "window", true),
                    ParseValue("subsystems", "audio", true)};
   } catch (std::exception e) {
      // Couldn't find the config file! Log a warning and generate defaults
      TR_WARN("Failed loading engine.toml with error: {}", e.what());
      TR_WARN("Falling back on default config settings...");
      subsystems = Subsystems();
   }
}