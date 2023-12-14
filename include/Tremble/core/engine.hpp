#pragma once

#include "tremble_export.hpp"

namespace TR {

class TREMBLE_EXPORT Engine {
  public:
   Engine();

   void Update(double delta);
};

} // namespace TR