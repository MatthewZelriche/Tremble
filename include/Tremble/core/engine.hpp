#pragma once

#include <tl/optional.hpp>

#include "tremble_export.hpp"
#include "Tremble/io/config.hpp"
#include "Tremble/io/window.hpp"

namespace TR {

class TREMBLE_EXPORT Engine {
  public:
   Engine();

   void Update(double delta);

  private:
   void InitializeSubsystems(const Subsystems &subsystemsConfig);

   tl::optional<Window> window = tl::nullopt;
};

} // namespace TR